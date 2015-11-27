#include "mainwindow.h"
#include "settingsdialog.h"
#include "ui_mainwindow.h"
#include "dds.h"
#include "vna_serial.h"
#include "dds_dg8saq_usb.h"
#include <iostream>

#define X2TO32 4294967296

using namespace std;

QSettings settings("HamRadio", "qVFO");

SettingsDialog *settingsdialog;
DDS *device;

quint64 frequency;
bool running;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    running = false;

    ReadSettings();
    setLimits();

    if (settingsdialog->vfo.configured)
    {
        StartHW();
    }

    /* connect signals and slots */
    connect(ui->freqCtrl, SIGNAL(newFrequency(qint64)), this, SLOT(setNewFrequency(qint64)));
}

MainWindow::~MainWindow()
{
    WriteSettings();

    delete ui;
}

void MainWindow::ReadSettings()
{
    settings.beginGroup("VFO");
    settingsdialog->vfo.DDSclock = settings.value("DDS_Clock", 125.0).toDouble();
    settingsdialog->vfo.fmax = settings.value("Max_Frequency", 50.0).toDouble();
    settingsdialog->vfo.fmin = settings.value("Min_Frequency", 0.00).toDouble();
    settingsdialog->vfo.mode = settings.value("Device_Mode", "").toString();
    settingsdialog->vfo.modeN = settings.value("Device_Mode_Index", 0).toInt();
    settingsdialog->vfo.port = settings.value("IO_ADdress", 0x278).toInt();
    settingsdialog->vfo.portN = settings.value("IO_ADdress_Index", 0).toInt();
    settingsdialog->vfo.serialportN = settings.value("Serial_Port_Index", 0).toInt();
    settingsdialog->vfo.serialport = settings.value("Serial_Port", "ttyS0").toString();
    settingsdialog->vfo.configured = settings.value("Configured", false).toBool();
    settingsdialog->vfo.multiplier = settings.value("Multiplier", false).toBool();
    frequency = settings.value("Frequency",10000000).toULongLong();
    settings.endGroup();
}

void MainWindow::WriteSettings()
{
    settings.beginGroup("VFO");
    settings.setValue("DDS_Clock", settingsdialog->vfo.DDSclock);
    settings.setValue("Max_Frequency", settingsdialog->vfo.fmax);
    settings.setValue("Min_Frequency", settingsdialog->vfo.fmin);
    settings.setValue("Device_Mode_Index", settingsdialog->vfo.modeN);
    settings.setValue("Device_Mode", settingsdialog->vfo.mode);
    settings.setValue("IO_Address_Index", settingsdialog->vfo.portN);
    settings.setValue("IO_Address", settingsdialog->vfo.port);
    settings.setValue("Serial_Port_Index", settingsdialog->vfo.serialportN);
    settings.setValue("Serial_Port", settingsdialog->vfo.serialport);
    settings.setValue("Configured", settingsdialog->vfo.configured);
    settings.setValue("Multiplier", settingsdialog->vfo.multiplier);
    settings.setValue("Frequency", frequency);
    settings.endGroup();
}


void MainWindow::on_actionConfigure_triggered()
{
    settingsdialog = new SettingsDialog(this);
    settingsdialog->show();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about ( this, tr("About qVFO"),
    tr("This is qVFO v 0.0.1, the software to control \n"
        "AD9850/51 DDS modules and network analyzers\n"
        "working as variable frequency generators\n\n"
        "(C)2014 Wojciech Kazubski <wk@ire.pw.edu.pl\n\n"
        "Licensed under GNU GPL v3 " ));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt ( this, tr("About Qt"));
}

void MainWindow::on_actionExit_triggered()
{
    WriteSettings();
    MainWindow::close();
}

void MainWindow::setNewFrequency(qint64 freq)
{
    quint32 n;
    double f;
    frequency = freq;
    f = freq;
    n = (quint32) (f * X2TO32 / (settingsdialog->vfo.DDSclock * 1e6)) ;
//    cout << "F=" << freq << " n=" << n << endl;
    if (running)
        device->setFrequency(n);
}

void MainWindow::setLimits()
{
    quint64 fmin, fmax;
    fmin = (quint64) (settingsdialog->vfo.fmin * 1e6);
    fmax = (quint64) (settingsdialog->vfo.fmax * 1e6);
    ui->freqCtrl->setup(10, fmin, fmax, 1, UNITS_MHZ);

    /* frequency control widget */
    ui->freqCtrl->setFrequency(frequency);
}

void MainWindow::StartHW()
{
    int result = 0;
    QString portname;
    cout << "Starting hardware..." << endl;
    if (running)
    {
        cout << "Closing previous hardware..." << endl;
        device->closePort();
    }
//    cout << "mode: " << settingsdialog->vfo.modeN << endl;
    switch (settingsdialog->vfo.modeN)
    {
    case 0:
        device = new DDS_DG8SAQ();
        portname = tr("AD9850 via DG8SAQ USB interface");
        break;
    case 1:
        device = new VnaSerial();
        portname = settingsdialog->vfo.serialport;
        break;
    default:
        result = -3;
    }
    if (result == 0)
        result = device->initDDS(settingsdialog->vfo.port, settingsdialog->vfo.serialport, settingsdialog->vfo.multiplier);
    switch (result)
    {
    case 0:
        ui->statusBar->showMessage(tr("Connected to %1").arg(portname));
        break;
    case -1:
        QMessageBox::critical(this, tr("Error"),
                              tr("Can't configure port: %1,\n"
                                 "error code: %2")
                              .arg(portname).arg(device->getError()));
        ui->statusBar->showMessage(tr("Configure error"));
        break;
    case -2:
        QMessageBox::critical(this, tr("Error"),
                              tr("Can't open port: %1,\n"
                                 "error code: %2")
                              .arg(portname).arg(device->getError()));
        ui->statusBar->showMessage(tr("Open error"));
        break;
    case -3:
        QMessageBox::critical(this, tr("Error"),
                              tr("Driver not available: %1")
                              .arg(portname));
        ui->statusBar->showMessage(tr("Not available"));
        break;
    case -4:
        QMessageBox::critical(this, tr("Error"),
                              tr("Cannot initialize USB"));
        ui->statusBar->showMessage(tr("Cannot initialize USB"));
        break;
    case -5:
        QMessageBox::critical(this, tr("Error"),
                              tr("Cannot get device list"));
        ui->statusBar->showMessage(tr("Cannot get device list"));
        break;
    case -6:
        QMessageBox::critical(this, tr("Error"),
                              tr("Can't open device: %1")
                              .arg(portname));
        ui->statusBar->showMessage(tr("Open error"));
        break;
    case -7:
        QMessageBox::critical(this, tr("Error"),
                              tr("Cannot claim interface: %1")
                              .arg(portname));
        ui->statusBar->showMessage(tr("Cannot claim interface"));
        break;
    }
    if (result == 0)
    {
        cout << "success" << endl;
        running = true;
        setNewFrequency(frequency);
    }
    else
        settingsdialog->vfo.configured = false;
}

void MainWindow::on_QuitPushButton_clicked()
{
    if (running)
        device->closePort();
    WriteSettings();
    MainWindow::close();
}

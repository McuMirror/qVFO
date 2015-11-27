#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <serialportinfo.h>
#include <iostream>

using namespace std;

QT_USE_NAMESPACE_SERIALPORT

VFOStruct SettingsDialog::vfo;
VFOStruct vfo_new;

bool ready;
bool device_changed;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    SetInitialValues();
    UpdateWindow();
    cout << "configuring..." << endl;
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::SetInitialValues()
{
    ready = false;
    device_changed = false;
    vfo_new.DDSclock = vfo.DDSclock;
    vfo_new.fmax = vfo.fmax;
    vfo_new.fmin = vfo.fmin;
    vfo_new.mode = vfo.mode;
    vfo_new.modeN = vfo.modeN;
    vfo_new.port = vfo.port;
    vfo_new.portN = vfo.portN;
    vfo_new.serialport = vfo.serialport;
    vfo_new.serialportN = vfo.serialportN;
    vfo_new.multiplier = vfo.multiplier;

    ui->DeviceComboBox->setCurrentIndex(vfo.modeN);
    ui->SerialComboBox->setCurrentIndex(fillPortsInfo());
    ui->ClockDoubleSpinBox->setValue(vfo.DDSclock);
    ui->MinFrequencyDoubleSpinBox->setValue(vfo.fmin);
    ui->MaxFrequencyDoubleSpinBox->setValue(vfo.fmax);
    ui->MultiplierCheckBox->setChecked(vfo.multiplier);
    ready = true;
}

void SettingsDialog::UpdateWindow()
{
    int x;
    x = ui->DeviceComboBox->currentIndex();
    ui->SerialComboBox->setEnabled(x == 1);
    ui->MultiplierCheckBox->setEnabled(x == 0);
}

int SettingsDialog::fillPortsInfo()
{
    int i, k;
    i = 0;
    k = 0;
    ui->SerialComboBox->clear();
    foreach (const SerialPortInfo &info, SerialPortInfo::availablePorts()) {
        QStringList list;
        list << info.portName() << info.description()
             << info.manufacturer() << info.systemLocation()
             << info.vendorIdentifier() << info.productIdentifier();

        ui->SerialComboBox->addItem(list.first(), list);
        if (info.portName() == vfo.serialport)
            k=i;
        i++;
    }
    return (k);
}

void SettingsDialog::on_DeviceComboBox_currentIndexChanged(int index)
{
    if (ready)
    {
        vfo_new.modeN = index;
        vfo_new.mode = ui->DeviceComboBox->currentText();
        UpdateWindow();
        device_changed = true;
    }
}

void SettingsDialog::on_ClockDoubleSpinBox_editingFinished()
{
    if (ready)
    {
        vfo_new.DDSclock = ui->ClockDoubleSpinBox->value();
    }
}

void SettingsDialog::on_MinFrequencyDoubleSpinBox_editingFinished()
{
    if (ready)
    {
        vfo_new.fmin = ui->MinFrequencyDoubleSpinBox->value();
    }
}

void SettingsDialog::on_MaxFrequencyDoubleSpinBox_editingFinished()
{
    if (ready)
    {
        vfo_new.fmax = ui->MaxFrequencyDoubleSpinBox->value();
    }
}

void SettingsDialog::on_SerialComboBox_currentIndexChanged(int index)
{
    if (ready)
    {
        vfo_new.serialportN = index;
        vfo_new.serialport = ui->SerialComboBox->currentText();
        device_changed = true;
    }
}

void SettingsDialog::on_MultiplierCheckBox_clicked(bool checked)
{
    if (ready)
    {
        vfo_new.multiplier = checked;
    }
}

void SettingsDialog::on_buttonBox_accepted()
{
//    cout << "config done" << device_changed << endl;

    vfo.DDSclock = vfo_new.DDSclock;
    vfo.fmax = vfo_new.fmax;
    vfo.fmin = vfo_new.fmin;
    vfo.mode = vfo_new.mode;
    vfo.modeN = vfo_new.modeN;
    vfo.port = vfo_new.port;
    vfo.portN = vfo_new.portN;
    vfo.serialport = vfo_new.serialport;
    vfo.serialportN = vfo_new.serialportN;
    vfo.multiplier = vfo_new.multiplier;
    vfo.configured = true;

    MainWindow *mdiparent = qobject_cast<MainWindow*>(this->parentWidget());
    mdiparent->setLimits();
    if (device_changed)
    {
        mdiparent->StartHW();
        device_changed = false;
    }
}

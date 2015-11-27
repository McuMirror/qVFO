#include "vna_serial.h"
#include <QtAddOnSerialPort/serialport.h>
#include <iostream>

using namespace std;

QT_USE_NAMESPACE_SERIALPORT

QString devicestr;
SerialPort *serial;
bool isOpen;

VnaSerial::VnaSerial()
{
    serial = new SerialPort();
    isOpen = false;
}

VnaSerial::~VnaSerial()
{
}

int VnaSerial::initDDS(int port_init, QString device_init, bool multiplier)
{
    devicestr = "/dev/" + device_init;
    cout << " VNA init: " << devicestr.toStdString() << endl;
    return(openSerialPort());
}

void VnaSerial::setFrequency(unsigned int n)
{
    char a;
    int i;

    SendParam(0);
    SendParam(n);
    SendParam(1);
    SendParam(0);
    serial->waitForBytesWritten(10);
    serial->waitForReadyRead(10);
    for (i=0;i<4;i++)
        serial->getChar(&a);
}

void VnaSerial::closePort()
{
    closeSerialPort();
}

int VnaSerial::getError()
{
    return (serial->error());
}

int VnaSerial::openSerialPort()
{
    int result = 0;
    serial->setPort(devicestr);
    if (serial->open(QIODevice::ReadWrite))
    {
        isOpen = true;
        if (serial->setRate(SerialPort::Rate115200)
                && serial->setDataBits(SerialPort::Data8)
                && serial->setParity(SerialPort::NoParity)
                && serial->setStopBits(SerialPort::OneStop)
                && serial->setFlowControl(SerialPort::NoFlowControl))
        {
            cout << "OK" << endl;
        }
        else
        {
            serial->close();
            isOpen = false;
            result = -1;
        }
    }
    else
    {
        result = -2;
    }
    cout << result << endl;
    return (result);
}

void VnaSerial::closeSerialPort()
{
    if (isOpen)
        serial->close();
    isOpen = false;
/*
    ui->statusBar->showMessage(tr("Disconnected"));
*/
}

void VnaSerial::SendParam(unsigned int x)
{
    QByteArray s = QByteArray::number(x);
    cout << "Out: " << s.data() << endl;
    serial->write(s + "\r");
}

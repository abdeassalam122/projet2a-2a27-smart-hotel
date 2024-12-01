#include "arduino.h"

Arduino::Arduino()
{
    dataBuffer = "";
    arduinoPortName = "";
    arduinoIsAvailable = false;
    serial = new QSerialPort;
}
QString Arduino::getPortName()
{
    return arduinoPortName;
}

QSerialPort* Arduino::getSerial()
{
    return serial;
}

int Arduino::connectToArduino()
{
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        qDebug() << "Port Name:" << serialPortInfo.portName();
        qDebug() << "Vendor ID:" << serialPortInfo.vendorIdentifier();
        qDebug() << "Product ID:" << serialPortInfo.productIdentifier();

        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            if (serialPortInfo.vendorIdentifier() == vendorID && serialPortInfo.productIdentifier() == productID) {
                arduinoIsAvailable = true;
                arduinoPortName = serialPortInfo.portName();
                break;
            }
        }
    }

    if (arduinoIsAvailable) {
        serial->setPortName(arduinoPortName);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            qDebug() << "Connected to Arduino on port:" << arduinoPortName;
            return 0; // Success
        } else {
            qDebug() << "Failed to open serial port:" << serial->errorString();
            return 1; // Port open failure
        }
    }

    qDebug() << "Arduino not found. Check connection or device IDs.";
    return -1; // Arduino not available
}


int Arduino::disconnectFromArduino()
{
    if (serial && serial->isOpen()) {
        serial->close();
        return 0;
    }
    return 1;
}


int Arduino::sendDataToArduino(const QString &data)
{
    if (serial->isWritable()) {
        serial->write(data.toUtf8());
        return 0;
    } else {
        qDebug() << "Failed to write to Arduino!";
        return 1;
    }
}

QByteArray Arduino::receiveDataFromArduino()
{
    if (serial->isReadable()) {
        dataBuffer = serial->readAll();
    }
    return dataBuffer;
}

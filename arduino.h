#ifndef ARDUINO_H
#define ARDUINO_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QSqlQuery>

class Arduino
{
public:
    Arduino();
    int connectToArduino();
    int disconnectFromArduino();
    int sendDataToArduino(const QString &data);
    QByteArray receiveDataFromArduino();
    QString getPortName();
    QSerialPort* getSerial();

private:
    QSerialPort *serial;              // Serial port object
    QString arduinoPortName;          // Port name for the Arduino
    bool arduinoIsAvailable;          // Arduino availability status
    QByteArray dataBuffer;            // Data buffer for incoming data

    static const quint16 vendorID = 9025;  // Vendor ID for Arduino Uno
    static const quint16 productID = 67;   // Product ID for Arduino Uno
};

#endif // ARDUINO_H

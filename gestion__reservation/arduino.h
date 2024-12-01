#ifndef ARDUINO_H
#define ARDUINO_H

#include <QSerialPort>

class Arduino
{
public:
    Arduino();
    ~Arduino();

    QSerialPort* getserial();      // Retourne l'objet QSerialPort
    QByteArray read_from_arduino(); // Lit les données depuis Arduino
    void write_to_arduino(const QByteArray &data); // Écrit des données vers Arduino

private:
    QSerialPort *serial;  // Pointeur vers l'objet QSerialPort
};

#endif // ARDUINO_H

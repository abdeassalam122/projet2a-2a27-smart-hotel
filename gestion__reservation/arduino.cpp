#include "arduino.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>  // Ajouter cet en-tête

Arduino::Arduino()
{
    serial = new QSerialPort();
    bool portTrouve = false;

    // Tentative de détection automatique des ports
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.vendorIdentifier() == 0x2341) {  // Exemple : ID de l'Arduino Uno
            serial->setPort(info);
            portTrouve = true;
            qDebug() << "Port trouvé automatiquement : " << info.portName();
            break;
        }
    }

    // Si aucun port détecté automatiquement, forcer l'utilisation de "COM10"
    if (!portTrouve) {
        qDebug() << "Aucun port détecté automatiquement. Tentative d'utilisation de COM10.";
        serial->setPortName("COM10");  // Forcer l'utilisation de COM10
    }

    // Ouverture du port série
    if (!serial->isOpen()) {
        if (!serial->open(QIODevice::ReadWrite)) {
            qDebug() << "Impossible d'ouvrir le port série. Code d'erreur: " << serial->error();
        } else {
            qDebug() << "Port série ouvert avec succès : " << serial->portName();
        }
    }

    // Configuration du port série
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
}


Arduino::~Arduino()
{
    if (serial->isOpen()) {
        serial->close();
    }
    delete serial;
}

QSerialPort* Arduino::getserial()
{
    return serial;
}

QByteArray Arduino::read_from_arduino()
{
    return serial->readAll();  // Retourne toutes les données lues depuis Arduino
}

void Arduino::write_to_arduino(const QByteArray &data)
{
    serial->write(data);  // Envoie des données à Arduino
}

#ifndef ARDUINO_QT_H
#define ARDUINO_QT_H

#include <QDialog>
#include "arduino.h"

// Déclaration avant-propos de Ui::Arduino_Qt
namespace Ui {
    class Arduino_Qt;
}

class Arduino;

class Arduino_Qt : public QDialog
{
    Q_OBJECT

public:
    explicit Arduino_Qt(QWidget *parent = nullptr, Arduino *arduinoInstance = nullptr);
    ~Arduino_Qt();

private:
    Ui::Arduino_Qt *ui; // Pointeur vers l'interface générée
    Arduino *arduino;

    void connectToArduino();
    void readFromArduino();
    void searchReservation(const QString &reservationID);
};

#endif // ARDUINO_QT_H

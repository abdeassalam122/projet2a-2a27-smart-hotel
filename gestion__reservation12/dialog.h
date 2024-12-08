#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSqlError>
#include "ui_dialog.h"
#include "arduino.h"    // Inclure la classe Arduino si nécessaire

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void readFromArduino();  // Déclaration de la méthode pour lire depuis Arduino
    void searchReservation(const QString &reservationID);  // Déclaration de la méthode searchReservation
private:
    Ui::Dialog *ui;
    Arduino *arduino;  // Déclaration de l'objet Arduino

    QString buffer;  // Déclarer un buffer pour stocker les caractères reçus


};

#endif // DIALOG_H

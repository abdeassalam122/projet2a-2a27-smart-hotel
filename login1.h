#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "arduino.h"
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_login_clicked();
   // void on_card_login(); // Slot to handle RFID card input
    void readFromArduino();

private:
    Ui::Login *ui;
    void validateCardID(const QString &cardID);
};

#endif // LOGIN_H

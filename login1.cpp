#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>


//QSerialPort *serial;

Login::Login(QWidget *parent)
    : QDialog(parent), ui(new Ui::Login)
{
    ui->setupUi(this);
    // Setup serial communication for RFID
  /*  serial = new QSerialPort(this);
    serial->setPortName("COMx"); // Replace COMx with the actual port
    serial->setBaudRate(QSerialPort::Baud9600);
    if (serial->open(QIODevice::ReadOnly)) {
        connect(serial, &QSerialPort::readyRead, this, &Login::on_card_login);
    } else {
        QMessageBox::critical(this, "Error", "Failed to open serial port.");
    }*/


}

Login::~Login()
{
    delete ui;

}

void Login::readFromArduino()
{
    QByteArray data = A.read_from_arduino();
    QString uid = QString::fromUtf8(data).trimmed(); // Convertir les données en chaîne
    if (!uid.isEmpty())
    {
        //qDebug() << "UID reçu:" << uid;
        if (uid != "/")
        {
            A.code += uid;
        }
        else
        {
            A.code.remove("/");
            QSqlQuery query;
            query.prepare("SELECT EMAIL,PASSWORD FROM EMPLOYE WHERE RFID_CARD_ID = :code");
            query.bindValue(":code", A.code);
            if (query.exec() && query.next())
            {
                QString email = query.value(0).toString();
                QString password = query.value(1).toString();



                        QMessageBox::information(this, "Succès", "L'UID existe  !");
                        A.code = "";
            ui->email->setText(email);
            ui->password->setText(password);
                    }
                    else
                    {
                    // UID does not exist
                        QMessageBox::warning(this, "Erreur", "L'UID n'existe pas !");
                        A.code = "";
                    }

                }
            }




}
void Login::on_login_clicked()
{
    QString email = ui->email->text().toLower().trimmed();
    QString password = ui->password->text().toLower().trimmed();

    if (!email.contains("@smarthotel")) {
        QMessageBox::warning(this, "Authentication", "Invalid email format.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE email = :email AND password = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            QMessageBox::information(this, "Authentication", "Login successful!");
            accept();
        } else {
            QMessageBox::warning(this, "Authentication", "Invalid email or password.");
        }
    } else {
        QMessageBox::critical(this, "Error", "Query error: " + query.lastError().text());
    }
}
/*
void Login::on_card_login()
{
    QByteArray data = serial->readAll();
    QString cardID = QString::fromUtf8(data).trimmed();
    if (!cardID.isEmpty()) {
        validateCardID(cardID);
    }
}*/

void Login::validateCardID(const QString &cardID)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE card_id = :cardID");
    query.bindValue(":cardID", cardID);

    if (query.exec()) {
        if (query.next()) {
            QMessageBox::information(this, "Authentication", "Card login successful!");
            accept();
        } else {
            QMessageBox::warning(this, "Authentication", "Invalid card ID.");
        }
    } else {
        QMessageBox::critical(this, "Error", "Query error: " + query.lastError().text());
    }
}

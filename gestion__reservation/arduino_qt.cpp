#include "arduino_qt.h"
#include "ui_arduino_qt.h"
#include <QMessageBox>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>

Arduino_Qt::Arduino_Qt(QWidget *parent, Arduino *arduinoInstance) :
    QDialog(parent),
    ui(new Ui::Arduino_Qt),
    arduino(arduinoInstance)
{
    ui->setupUi(this);
    connectToArduino();

    connect(arduino->getserial(), &QSerialPort::readyRead, this, &Arduino_Qt::readFromArduino);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Arduino_Qt::readFromArduino);
    timer->start(50);
}

Arduino_Qt::~Arduino_Qt()
{
    delete ui;
}

void Arduino_Qt::connectToArduino()
{
    int result = arduino->connect_arduino();
    if (result == 0) {
        qDebug() << "Arduino connecté!";
    } else if (result == 1) {
        QMessageBox::critical(this, "Erreur", "Échec de l'ouverture du port série!");
    } else {
        QMessageBox::critical(this, "Erreur", "Arduino non trouvé!");
    }
}

void Arduino_Qt::readFromArduino()
{
    QByteArray data = arduino->read_from_arduino(); // Lire les données depuis Arduino
    if (!data.isEmpty()) {
        QString receivedText = QString::fromUtf8(data); // Convertir les données en QString
        for (QChar c : receivedText) {
            if (c == '#') {
                // Une commande terminée, rechercher dans la base de données
                ui->lineEdit->setText(buffer.trimmed());
                searchReservation(buffer.trimmed());
                buffer.clear();
            } else if (c == '*') {
                // Gérer la suppression
                QString currentText = ui->lineEdit->text();
                if (!currentText.isEmpty()) {
                    currentText.chop(1);  // Supprimer le dernier caractère
                    ui->lineEdit->setText(currentText);
                    buffer.chop(1);  // Supprimer le dernier caractère du buffer
                }
            } else if (!c.isSpace()) {
                buffer.append(c);  // Ajouter le caractère au buffer
            }
        }
    }
}

void Arduino_Qt::searchReservation(const QString &reservationID)
{
    if (reservationID.isEmpty()) {
        ui->labelResult->setText("ID de réservation vide.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_R, PRIX, DEBUT_SEJOUR, FIN_SEJOUR, CHAMBRE, DATE_RES FROM reservations WHERE ID_R = :reservationID");
    query.bindValue(":reservationID", reservationID);

    if (query.exec()) {
        if (query.next()) {
            // Récupérer les valeurs de la base de données
            QString id = query.value("ID_R").toString();
            QString prix = query.value("PRIX").toString();
            QString debut = query.value("DEBUT_SEJOUR").toString();
            QString fin = query.value("FIN_SEJOUR").toString();
            QString chambre = query.value("CHAMBRE").toString();
            QString dateRes = query.value("DATE_RES").toString();

            // Afficher les résultats dans le label
            ui->labelResult->setText("ID: " + id + "\nPrix: " + prix +
                                     "\nDébut: " + debut +
                                     "\nFin: " + fin +
                                     "\nChambre: " + chambre +
                                     "\nDate de réservation: " + dateRes);
        } else {
            ui->labelResult->setText("Aucune réservation trouvée.");
        }
    } else {
        qDebug() << "Erreur de requête SQL : " << query.lastError().text();
        ui->labelResult->setText("Erreur de recherche dans la base de données.");
    }
}

void Arduino_Qt::on_pushButtonSend_clicked()
{
    QByteArray dataToSend = ui->lineEdit->text().toUtf8();
    if (!dataToSend.isEmpty()) {
        arduino->write_to_arduino(dataToSend);
        qDebug() << "Données envoyées à Arduino : " << dataToSend;
    } else {
        QMessageBox::warning(this, "Avertissement", "Aucune donnée à envoyer.");
    }
}

void Arduino_Qt::on_pushButtonDisconnect_clicked()
{
    if (arduino->close_arduino() == 0) {
        qDebug() << "Arduino déconnecté!";
    } else {
        qDebug() << "Échec de la déconnexion!";
        QMessageBox::critical(this, "Erreur", "Impossible de fermer la connexion avec Arduino.");
    }
}

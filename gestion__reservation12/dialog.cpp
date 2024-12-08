/*#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>  // Ajoutez cette ligne

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    arduino(new Arduino)  // Initialisation de l'objet Arduino
{
    qDebug() << "Construction de Dialog commencée.";
    ui->setupUi(this);
    qDebug() << "Interface Dialog initialisée avec succès.";

    // Vérifier si 'arduino' est correctement initialisé avant de l'utiliser
    if (arduino->getserial() && arduino->getserial()->isOpen()) {
        qDebug() << "Connexion à Arduino activée.";
        connect(arduino->getserial(), &QSerialPort::readyRead, this, &Dialog::readFromArduino);
    } else {
        qDebug() << "Erreur : Arduino non initialisé ou port série non valide.";
    }
}

Dialog::~Dialog()
{
    qDebug() << "Destruction de Dialog.";
    delete arduino;  // Libération de la mémoire
    delete ui;
}

/*void Dialog::readFromArduino()
{
    // Vérifier si le port série est disponible et ouvert
    if (!arduino->getserial() || !arduino->getserial()->isOpen()) {
        qDebug() << "Erreur : Port série non disponible.";
        return;
    }

    // Lire les données envoyées par Arduino
    QByteArray data = arduino->read_from_arduino();
    if (!data.isEmpty()) {
        QString receivedText = QString::fromUtf8(data);  // Convertir les données en QString

        qDebug() << "Données reçues : " << receivedText;  // Affichage des données dans la console pour débogage

        for (QChar c : receivedText) {
            // Ignore les retours à la ligne et les caractères non numériques
            if (c == '\r' || c == '\n') {
                continue;  // Ignorer le retour à la ligne
            }

            if (c == '#') {
                // Lorsque # est appuyé, valider le code et effectuer la recherche
                ui->lineEdit->setText(buffer.trimmed());  // Mettre à jour le QLineEdit avec l'ID complet
                searchReservation(buffer.trimmed());  // Recherche de la réservation
                buffer.clear();  // Réinitialiser le buffer pour le prochain ID
                qDebug() << "Recherche effectuée avec l'ID : " << buffer;
            } else if (c == '*') {
                // Si '*' est trouvé, supprimer le dernier caractère
                if (!buffer.isEmpty()) {
                    buffer.chop(1);  // Supprimer le dernier caractère du buffer
                    ui->lineEdit->setText(buffer);  // Mettre à jour le QLineEdit avec le buffer actuel
                    qDebug() << "Dernier caractère supprimé. Nouveau code : " << buffer;  // Optionnel: affichage pour le débogage
                }
            } else {
                // Ajouter les caractères reçus dans le buffer
                buffer.append(c);
                ui->lineEdit->setText(buffer);  // Mettre à jour le QLineEdit pendant la saisie
            }
        }
    }
}

void Dialog::searchReservation(const QString &reservationID)
{
    QSqlQuery query;
    query.prepare("SELECT ID_R, PRIX, DEBUT_SEJOUR, FIN_SEJOUR, CHAMBRE, DATE_RES FROM reservations WHERE ID_R = :reservationID");
    query.bindValue(":reservationID", reservationID);

    if (query.exec()) {
        if (query.next()) {
            // Si une réservation est trouvée, afficher les détails
            QString id = query.value("ID_R").toString();
            QString prix = query.value("PRIX").toString();
            QString debut = query.value("DEBUT_SEJOUR").toString();
            QString fin = query.value("FIN_SEJOUR").toString();
            QString chambre = query.value("CHAMBRE").toString();
            QString dateRes = query.value("DATE_RES").toString();

            ui->labelResult->setText("La réservation est trouvée !\nID: " + id + "\nPrix: " + prix +
                                     "\nDébut: " + debut +
                                     "\nFin: " + fin +
                                     "\nChambre: " + chambre +
                                     "\nDate de réservation: " + dateRes);
        } else {
            // Si aucune réservation n'est trouvée
            ui->labelResult->setText("Aucune réservation trouvée.");
        }
    } else {
        qDebug() << "Erreur de requête : " << query.lastError().text();  // Affiche l'erreur dans la console si la requête échoue
    }
}
void Dialog::readFromArduino() {
    // Vérifier si le port série est disponible et ouvert
    if (!arduino->getserial() || !arduino->getserial()->isOpen()) {
        qDebug() << "Erreur : Port série non disponible.";
        return;
    }

    // Lire les données envoyées par Arduino
    QByteArray data = arduino->read_from_arduino();
    if (!data.isEmpty()) {
        QString receivedText = QString::fromUtf8(data);  // Convertir les données en QString

        qDebug() << "Données reçues : " << receivedText;  // Affichage des données dans la console pour débogage

        for (QChar c : receivedText) {
            // Ignore les retours à la ligne et les caractères non numériques
            if (c == '\r' || c == '\n') {
                continue;  // Ignorer le retour à la ligne
            }

            if (c == '#') {
                // Quand '#' est appuyé, valider le code et effectuer la recherche
                ui->lineEdit->setText(buffer.trimmed());  // Mettre à jour le QLineEdit avec l'ID complet
                searchReservation(buffer.trimmed());  // Recherche de la réservation
                buffer.clear();  // Réinitialiser le buffer pour le prochain ID
                qDebug() << "Recherche effectuée avec l'ID : " << buffer;
            } else if (c == '*') {
                // Si '*' est trouvé, supprimer le dernier caractère du code
                if (!buffer.isEmpty()) {
                    buffer.chop(1);  // Supprimer le dernier caractère du buffer
                    ui->lineEdit->setText(buffer);  // Mettre à jour le QLineEdit avec le buffer actuel
                    qDebug() << "Dernier caractère supprimé. Nouveau code : " << buffer;  // Affichage pour le débogage
                }
            } else {
                // Ajouter les caractères reçus dans le buffer
                buffer.append(c);
                ui->lineEdit->setText(buffer);  // Mettre à jour le QLineEdit pendant la saisie
            }
        }
    }
}

void Dialog::searchReservation(const QString &reservationID)
{
    QSqlQuery query;
    query.prepare("SELECT ID_R, PRIX, DEBUT_SEJOUR, FIN_SEJOUR, CHAMBRE, DATE_RES FROM reservations WHERE ID_R = :reservationID");
    query.bindValue(":reservationID", reservationID);

    if (query.exec()) {
        if (query.next()) {
            // Si une réservation est trouvée, afficher les détails
            QString id = query.value("ID_R").toString();
            QString prix = query.value("PRIX").toString();
            QString debut = query.value("DEBUT_SEJOUR").toString();
            QString fin = query.value("FIN_SEJOUR").toString();
            QString chambre = query.value("CHAMBRE").toString();
            QString dateRes = query.value("DATE_RES").toString();

            ui->labelResult->setText("La réservation est trouvée !\nID: " + id + "\nPrix: " + prix +
                                     "\nDébut: " + debut +
                                     "\nFin: " + fin +
                                     "\nChambre: " + chambre +
                                     "\nDate de réservation: " + dateRes);
        } else {
            // Si aucune réservation n'est trouvée
            ui->labelResult->setText("Aucune réservation trouvée.");
        }
    } else {
        qDebug() << "Erreur de requête : " << query.lastError().text();  // Affiche l'erreur dans la console si la requête échoue
    }
}*/
#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog),
    arduino(new Arduino)  // Initialisation de l'objet Arduino
{
    qDebug() << "Construction de Dialog commencée.";
    ui->setupUi(this);
    qDebug() << "Interface Dialog initialisée avec succès.";

    // Vérifier si 'arduino' est correctement initialisé avant de l'utiliser
    if (arduino->getserial() && arduino->getserial()->isOpen()) {
        qDebug() << "Connexion à Arduino activée.";
        connect(arduino->getserial(), &QSerialPort::readyRead, this, &Dialog::readFromArduino);
    } else {
        qDebug() << "Erreur : Arduino non initialisé ou port série non valide.";
    }
}

Dialog::~Dialog()
{
    qDebug() << "Destruction de Dialog.";
    delete arduino;  // Libération de la mémoire
    delete ui;
}

void Dialog::readFromArduino() {
    // Vérifier si le port série est disponible et ouvert
    if (!arduino->getserial() || !arduino->getserial()->isOpen()) {
        qDebug() << "Erreur : Port série non disponible.";
        return;
    }

    // Lire les données envoyées par Arduino
    QByteArray data = arduino->read_from_arduino();
    if (!data.isEmpty()) {
        QString receivedText = QString::fromUtf8(data).trimmed();  // Nettoyer les espaces et retours à la ligne

        qDebug() << "Données reçues nettoyées : " << receivedText;

        for (const QChar &c : receivedText) {
            // Ignore les retours à la ligne et les caractères non numériques
            if (c == '\r' || c == '\n') {
                continue;  // Ignorer le retour à la ligne
            }

            if (c == '#') {
                // Lors de l'appui sur #, valider et effectuer la recherche
                QString id = buffer.trimmed();
                if (!id.isEmpty()) {
                    qDebug() << "Validation et recherche pour l'ID : " << id;
                    ui->lineEdit->setText(id);  // Mettre à jour le LineEdit
                    searchReservation(id);  // Recherche dans la base de données
                } else {
                    qDebug() << "Aucun ID à valider.";
                }
                buffer.clear();  // Réinitialiser le buffer après validation
            } else if (c == '*') {
                // Gestion de suppression : on enlève le dernier caractère du buffer
                qDebug() << "Caractère '*' détecté. Suppression du dernier caractère.";
                if (!buffer.isEmpty()) {
                    buffer.chop(1);  // Supprimer le dernier caractère du buffer
                    ui->lineEdit->setText(buffer);  // Mettre à jour le LineEdit
                }
            } else {
                // Ajouter les caractères reçus dans le buffer
                buffer.append(c);
                qDebug() << "Caractère ajouté au buffer : " << c;
                ui->lineEdit->setText(buffer);  // Mise à jour continue du LineEdit
            }
        }

        // Empêcher les doublons en vidant les données répétées après traitement
        data.clear();
    } else {
        qDebug() << "Aucune donnée reçue.";
    }
}

void Dialog::searchReservation(const QString &reservationID) {
    if (reservationID.isEmpty()) {
        ui->labelResult->setText("ID vide !");  // Afficher un message si l'ID est vide
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM reservations WHERE ID_R = :id");
    query.bindValue(":id", reservationID);

    if (query.exec()) {
        if (query.next()) {
            // Si une réservation est trouvée, afficher les détails
            QString id = query.value("ID_R").toString();
            QString dateRes = query.value("DATE_RES").toString();
            ui->labelResult->setText("Réservation trouvée :\nID : " + id + "\nDate : " + dateRes);
        } else {
            ui->labelResult->setText("Aucune réservation trouvée !");
        }
    } else {
        ui->labelResult->setText("Erreur SQL !");
        qDebug() << "Erreur SQL : " << query.lastError().text();
    }
}

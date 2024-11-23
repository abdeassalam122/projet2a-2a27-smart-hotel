#include "gestion__reservation.h"  // Inclure le fichier d'en-tête correct
#include <QApplication>
#include <QMessageBox>
#include "connection.h"  // Inclure la classe de connexion à la base de données

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;  // Créer une instance de la classe Connection
    bool test = c.createConnection();  // Établir la connexion à la base de données

    gestion__reservation w;  // Créer une instance de la fenêtre de gestion des réservations
    if (test) {
        w.show();  // Afficher la fenêtre principale
        QMessageBox::information(nullptr, QObject::tr("Connexion à la base de données"),
                                 QObject::tr("Connexion réussie.\n"
                                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Connexion à la base de données"),
                              QObject::tr("Échec de la connexion.\n"
                                          "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }

    return a.exec();  // Exécuter l'application
}

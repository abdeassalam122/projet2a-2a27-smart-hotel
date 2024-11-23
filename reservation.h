#ifndef RESERVATION_H
#define RESERVATION_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Reservation
{
    int id_r;
    int prix;
    int date_res;
    int debut_sejour;
    int fin_sejour;
    QString chambre;

public:
    Reservation(); // Default constructor
    Reservation(int id, int prix, int date_res, int debut_sejour, int fin_sejour, QString chambre);

    int getID() const { return id_r; }
    int getPrix() const { return prix; }
    int getDateRes() const { return date_res; }
    int getDebutSejour() const { return debut_sejour; }
    int getFinSejour() const { return fin_sejour; }
    QString getChambre() const { return chambre; }

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier(int);
    QSqlQueryModel* tri(bool ascending = true);
    QSqlQueryModel* rechercheParID(int id);
    QMap<QString, double> getStatistics();
    QSqlQueryModel* historique();
};

#endif // RESERVATION_H

#ifndef RESERVATION_H
#define RESERVATION_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>   // Ajouter cette ligne pour inclure QDate

class Reservation
{
private:
    int id_r;
    int prix;
    QDate date_res;
    QDate debut_sejour;
    QDate fin_sejour;
    QString chambre;
public:
    Reservation(); // Constructeur par défaut
    Reservation(int id, int prix, QDate date_res, QDate debut_sejour, QDate fin_sejour, QString chambre);

    int getID() const { return id_r; }
    int getPrix() const { return prix; }
    QDate getDateRes() const { return date_res; }
    QDate getDebutSejour() const { return debut_sejour; }
    QDate getFinSejour() const { return fin_sejour; }
    QString getChambre() const { return chambre; }

    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int);
    bool modifier(int);
    QSqlQueryModel* tri(bool ascending = true);
    QSqlQueryModel* rechercheParID(int id);
    QMap<QString, double> getStatistics();
};


#endif // RESERVATION_H

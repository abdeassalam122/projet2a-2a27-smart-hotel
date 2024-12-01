#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QSqlQueryModel>
#include <QMap>
#include <QObject>

class Fournisseur
{
   int id;
    QString Nom_F, Adresse, Email;
    QString Telephone;
    QString Secteur;


public:
    Fournisseur();
    Fournisseur(int id, QString Nom_F, QString Adresse, QString Email, QString Telephone, QString Secteur);

    QString getNom_F() { return Nom_F; }
    QString getAdresse() { return Adresse; }
    QString getEmail() { return Email; }
    int getID() { return id; }
    QString getTelephone() const { return Telephone; }
    QString getSecteur() const { return Secteur; }


    void setNom_F(QString n) { Nom_F = n; }
    void setAdresse(QString a) { Adresse = a; }
    void setEmail(QString e) { Email = e; }
    void setID(int id) { this->id = id; }
    void setTelephone(QString tel) { Telephone = tel; }
    void setSecteur(QString secteur) { Secteur = secteur; }


    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier();
    QSqlQueryModel* rechercherParSecteur(const QString& secteur);
    QMap<QString, QMap<QString, double>> getStatistics();



};


#endif // FOURNISSEUR_H

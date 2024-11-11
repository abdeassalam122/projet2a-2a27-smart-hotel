#include "fournisseur.h"
#include <QSqlQuery>

Fournisseur::Fournisseur() : id(0), Nom_F(""), Adresse(""), Email(""), Telephone(""), Secteur("") {}

Fournisseur::Fournisseur(int id, QString Nom_F, QString Adresse, QString Email, QString Telephone, QString Secteur)
{
    this->id = id;
    this->Nom_F = Nom_F;
    this->Adresse = Adresse;
    this->Email = Email;
    this->Telephone = Telephone;
    this->Secteur = Secteur;
}

bool Fournisseur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO FOURNISSEUR (ID_F, NOM, ADRESSE, EMAIL, TELEPHONE,SECTEUR_ACTIVITE) VALUES (:ID_F, :NOM, :ADRESSE, :EMAIL, :TELEPHONE, :SECTEUR_ACTIVITE)");
    query.bindValue(":ID_F", id); // Assurez-vous d'utiliser l'attribut id
    query.bindValue(":NOM", Nom_F);
    query.bindValue(":ADRESSE", Adresse);
    query.bindValue(":EMAIL", Email);
    query.bindValue(":TELEPHONE", Telephone);
    query.bindValue(":SECTEUR_ACTIVITE", Secteur);
    return query.exec();
}

QSqlQueryModel* Fournisseur::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM FOURNISSEUR");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_F"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("SECTEUR_ACTIVITE"));
    return model;
}

bool Fournisseur::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FOURNISSEUR WHERE ID_F = :ID_F");
    query.bindValue(":ID_F", id);
    return query.exec();
}

bool Fournisseur::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE FOURNISSEUR SET NOM = :NOM, ADRESSE = :ADRESSE, EMAIL = :EMAIL, TELEPHONE = :TELEPHONE, SECTEUR_ACTIVITE = :SECTEUR_ACTIVITE WHERE ID_F = :ID_F");
    query.bindValue(":ID_F", id);
    query.bindValue(":NOM", Nom_F);
    query.bindValue(":ADRESSE", Adresse);
    query.bindValue(":EMAIL", Email);
    query.bindValue(":TELEPHONE", Telephone);
    query.bindValue(":SECTEUR_ACTIVITE", Secteur);
    return query.exec();



}


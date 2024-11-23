#include "reservation.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QObject>



// Default constructor
Reservation::Reservation() : id_r(0), prix(0), date_res(0), debut_sejour(0), fin_sejour(0), chambre("") {}

// Parameterized constructor
Reservation::Reservation(int id, int p, int d_res, int deb, int fin, QString ch)
    : id_r(id), prix(p), date_res(d_res), debut_sejour(deb), fin_sejour(fin), chambre(ch) {}

bool Reservation::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO RESERVATIONS (ID_R, PRIX, DATE_RES, DEBUT_SEJOUR, FIN_SEJOUR, CHAMBRE) "
                  "VALUES (:ID_R, :PRIX, :DATE_RES, :DEBUT_SEJOUR, :FIN_SEJOUR, :CHAMBRE)");
    query.bindValue(":ID_R", id_r);
    query.bindValue(":PRIX", prix);
    query.bindValue(":DATE_RES", date_res);
    query.bindValue(":DEBUT_SEJOUR", debut_sejour);
    query.bindValue(":FIN_SEJOUR", fin_sejour);
    query.bindValue(":CHAMBRE", chambre);

    return query.exec();
}

QSqlQueryModel * Reservation::afficher() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM RESERVATIONS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_R"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATE_RES"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DEBUT_SEJOUR"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("FIN_SEJOUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CHAMBRE"));

    return model;
}

bool Reservation::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM RESERVATIONS WHERE ID_R = :ID_R");
    query.bindValue(":ID_R", id);

    return query.exec();
}
bool Reservation::modifier(int id) {
    QSqlQuery query;
    query.prepare("UPDATE RESERVATIONS SET PRIX = :PRIX, DATE_RES = :DATE_RES, DEBUT_SEJOUR = :DEBUT_SEJOUR, "
                  "FIN_SEJOUR = :FIN_SEJOUR, CHAMBRE = :CHAMBRE WHERE ID_R = :ID_R");
    query.bindValue(":ID_R", id);
    query.bindValue(":PRIX", prix);
    query.bindValue(":DATE_RES", date_res);
    query.bindValue(":DEBUT_SEJOUR", debut_sejour);
    query.bindValue(":FIN_SEJOUR", fin_sejour);
    query.bindValue(":CHAMBRE", chambre);

    return query.exec();
}

QSqlQueryModel* Reservation::tri(bool ascending) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sortOrder = ascending ? "ASC" : "DESC";
    model->setQuery("SELECT * FROM RESERVATIONS ORDER BY DATE_RES " + sortOrder);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_R"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATE_RES"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DEBUT_SEJOUR"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("FIN_SEJOUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CHAMBRE"));
    return model;
}

QSqlQueryModel* Reservation::rechercheParID(int id) {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM RESERVATIONS WHERE ID_R = " + QString::number(id));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_R"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATE_RES"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DEBUT_SEJOUR"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("FIN_SEJOUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CHAMBRE"));
    return model;
}

QMap<QString, double> Reservation::getStatistics()
{
    QMap<QString, double> stats;
    QSqlQuery query;

    // Total price
    query.exec("SELECT SUM(PRIX) FROM RESERVATIONS");
    if (query.next()) {
        stats["total"] = query.value(0).toDouble();
    }

    // Average price
    query.exec("SELECT AVG(PRIX) FROM RESERVATIONS");
    if (query.next()) {
        stats["average"] = query.value(0).toDouble();
    }

    // Minimum price
    query.exec("SELECT MIN(PRIX) FROM RESERVATIONS");
    if (query.next()) {
        stats["min"] = query.value(0).toDouble();
    }

    // Maximum price
    query.exec("SELECT MAX(PRIX) FROM RESERVATIONS");
    if (query.next()) {
        stats["max"] = query.value(0).toDouble();
    }

    return stats;
}

// Function to retrieve all historical reservations (can be used in your UI)
QSqlQueryModel* Reservation::historique()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT ID_R, CHAMBRE, PRIX, DATE_RES, DEBUT_SEJOUR, FIN_SEJOUR FROM RESERVATIONS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_R"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CHAMBRE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_RES"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DEBUT_SEJOUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("FIN_SEJOUR"));
    return model;
}

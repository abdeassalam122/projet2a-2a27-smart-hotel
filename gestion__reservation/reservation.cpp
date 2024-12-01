#include "reservation.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QObject>

// Default constructor
Reservation::Reservation()
    : id_r(0), prix(0), date_res(QDate()), debut_sejour(QDate()), fin_sejour(QDate()), chambre("") {}

// Parameterized constructor
Reservation::Reservation(int id, int p, QDate d_res, QDate deb, QDate fin, QString ch)
    : id_r(id), prix(p), date_res(d_res), debut_sejour(deb), fin_sejour(fin), chambre(ch) {}

// Ajouter une réservation
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

// Afficher toutes les réservations
QSqlQueryModel* Reservation::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM RESERVATIONS");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_R"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATE_RES"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DEBUT_SEJOUR"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("FIN_SEJOUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CHAMBRE"));

    return model;
}

// Supprimer une réservation
bool Reservation::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM RESERVATIONS WHERE ID_R = :ID_R");
    query.bindValue(":ID_R", id);

    return query.exec();
}

// Modifier une réservation
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

// Trier les réservations
QSqlQueryModel* Reservation::tri(bool ascending) {
    QSqlQueryModel* model = new QSqlQueryModel();
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

// Recherche par ID
QSqlQueryModel* Reservation::rechercheParID(int id) {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM RESERVATIONS WHERE ID_R = " + QString::number(id));
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_R"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("DATE_RES"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DEBUT_SEJOUR"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("FIN_SEJOUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CHAMBRE"));

    return model;
}

// Statistiques des réservations
QMap<QString, double> Reservation::getStatistics() {
    QMap<QString, double> stats;
    QSqlQuery query;

    // Prix total
    query.exec("SELECT SUM(PRIX) FROM RESERVATIONS");
    if (query.next()) {
        stats["total"] = query.value(0).toDouble();
    }

    // Prix moyen
    query.exec("SELECT AVG(PRIX) FROM RESERVATIONS");
    if (query.next()) {
        stats["average"] = query.value(0).toDouble();
    }

    // Prix minimum
    query.exec("SELECT MIN(PRIX) FROM RESERVATIONS");
    if (query.next()) {
        stats["min"] = query.value(0).toDouble();
    }

    // Prix maximum
    query.exec("SELECT MAX(PRIX) FROM RESERVATIONS");
    if (query.next()) {
        stats["max"] = query.value(0).toDouble();
    }

    return stats;
}

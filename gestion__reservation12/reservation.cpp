#include "reservation.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QMap>
#include <QTextDocument>
#include <QPrinter>


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
// Statistiques des réservations par type de chambre
QMap<QString, int> Reservation::getStatisticsByRoomType() {
    QMap<QString, int> stats;
    QSqlQuery query;

    // Requête SQL pour compter les réservations par type de chambre
    query.exec("SELECT CHAMBRE, COUNT(*) AS nombre_reservations "
               "FROM RESERVATIONS "
               "WHERE CHAMBRE IN ('Chambre simple', 'Chambre double', 'Suite', 'Chambre avec balcon') "
               "GROUP BY CHAMBRE");

    // Stocker les résultats dans la structure QMap
    while (query.next()) {
        QString roomType = query.value("CHAMBRE").toString();
        int count = query.value("nombre_reservations").toInt();
        stats[roomType] = count;
    }

    return stats;
}

QMap<QString, double> Reservation::getStatistics() {
    QMap<QString, double> stats;
    QSqlQuery query;

    // Requête SQL pour obtenir les statistiques
    query.exec("SELECT CHAMBRE, SUM(PRIX) AS total FROM RESERVATIONS GROUP BY CHAMBRE");
    while (query.next()) {
        QString roomType = query.value("CHAMBRE").toString();
        double totalPrix = query.value("total").toDouble();
        stats[roomType] = totalPrix;
    }

    return stats;
}
bool Reservation::exportToPDF(const QString &filePath) {
    QTextDocument document;
    QString htmlContent;

    // Préparer une table HTML pour structurer les données
    htmlContent.append("<h1>Liste des Réservations</h1>");
    htmlContent.append("<table border='1' cellspacing='0' cellpadding='5'>");
    htmlContent.append("<tr><th>ID</th><th>Prix</th><th>Date de Réservation</th><th>Début de Séjour</th><th>Fin de Séjour</th><th>Chambre</th></tr>");

    // Récupérer les données depuis la base de données
    QSqlQuery query("SELECT * FROM RESERVATIONS");
    while (query.next()) {
        htmlContent.append("<tr>");
        htmlContent.append("<td>" + query.value("ID_R").toString() + "</td>");
        htmlContent.append("<td>" + query.value("PRIX").toString() + "</td>");
        htmlContent.append("<td>" + query.value("DATE_RES").toDate().toString("dd/MM/yyyy") + "</td>");
        htmlContent.append("<td>" + query.value("DEBUT_SEJOUR").toDate().toString("dd/MM/yyyy") + "</td>");
        htmlContent.append("<td>" + query.value("FIN_SEJOUR").toDate().toString("dd/MM/yyyy") + "</td>");
        htmlContent.append("<td>" + query.value("CHAMBRE").toString() + "</td>");
        htmlContent.append("</tr>");
    }
    htmlContent.append("</table>");

    // Définir le contenu HTML dans le document
    document.setHtml(htmlContent);

    // Configurer l'imprimante pour la sortie PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Imprimer le document dans le fichier PDF
    document.print(&printer);

    return true;  // Retourne true si la génération a réussi
}

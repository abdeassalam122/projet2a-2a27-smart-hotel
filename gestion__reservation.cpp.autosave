#include "gestion__reservation.h"
#include "ui_gestion__reservation.h"
#include "reservation.h"
#include <QMessageBox>
#include <QSqlQuery>         // For SQL queries
#include <QSqlQueryModel>    // For SQL query result models
#include <QtCharts/QChartView>       // For chart view
#include <QtCharts/QChart>           // For the chart
#include <QtCharts/QBarSeries>       // For the bar series
#include <QtCharts/QBarSet>          // For the bar set
#include <QtCharts/QBarCategoryAxis> // For X-axis categories
#include <QtCharts/QValueAxis>       // For Y-axis values

#include <QGraphicsScene>       // For managing the graphics scene
#include <QGraphicsView>        // For displaying the graphics scene
#include <QMessageBox>          // For showing message boxes
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QDebug>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QSqlError>
using namespace QtCharts;

gestion__reservation::gestion__reservation(QWidget *parent) :
    QMainWindow(parent),  // Initialise QMainWindow avec le parent
    ui(new Ui::gestion__reservation)
{
    ui->setupUi(this);  // Configure l'interface utilisateur

    // Afficher les réservations dans tableView (qui est ui->tableView)
    ui->tableView->setModel(Rtmp.afficher());
}


gestion__reservation::~gestion__reservation()
{
    delete ui;
}

void gestion__reservation::on_pushButton_clicked()
{
    int id_r = ui->lineEdit_ID->text().toInt();
    int prix = ui->lineEdit_Prix->text().toInt();
    int date_res = ui->lineEdit_DateRes->text().toInt();
    int debut_sejour = ui->lineEdit_DebutSejour->text().toInt();
    int fin_sejour = ui->lineEdit_FinSejour->text().toInt();
    QString chambre = ui->lineEdit_Chambre->text();

    Reservation R(id_r, prix, date_res, debut_sejour, fin_sejour, chambre);
    bool test = R.ajouter();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Ajout réussi"),
                    QObject::tr("La réservation a été ajoutée avec succès.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->tableView->setModel(Rtmp.afficher()); // Refresh reservation display
        updateHistorique("Add", "ID: " + QString::number(id_r) +  ", Chambre: " + chambre);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                 QObject::tr("L'ajout de la réservation a échoué.\n"
                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}

void gestion__reservation::on_pushButton_3_clicked()
{
    int id_r = ui->lineEdit_ID->text().toInt();
    bool test = Rtmp.supprimer(id_r);

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Suppression réussie"),
                    QObject::tr("La réservation a été supprimée avec succès.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->tableView->setModel(Rtmp.afficher()); // Refresh reservation display
        updateHistorique("Delete", "ID: " + QString::number(id_r) );
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                    QObject::tr("La suppression de la réservation a échoué.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}

void gestion__reservation::on_pushButton_2_clicked()
{
    int id_r = ui->lineEdit_ID->text().toInt();
    int prix = ui->lineEdit_Prix->text().toInt();
    int date_res = ui->lineEdit_DateRes->text().toInt();
    int debut_sejour = ui->lineEdit_DebutSejour->text().toInt();
    int fin_sejour = ui->lineEdit_FinSejour->text().toInt();
    QString chambre = ui->lineEdit_Chambre->text();

    Reservation R(id_r, prix, date_res, debut_sejour, fin_sejour, chambre);
    bool test = R.modifier(id_r); // Call the update function with the ID of the reservation

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Modification réussie"),
                    QObject::tr("La réservation a été modifiée avec succès.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->tableView->setModel(Rtmp.afficher()); // Refresh reservation display
        updateHistorique("Modify", "ID: " + QString::number(id_r) + ", New Chambre: " + chambre);

    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                    QObject::tr("La modification de la réservation a échoué.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}

void gestion__reservation::on_pushButton_T_clicked()
{
    static bool ascending = true; // Toggle flag
    ui->tableView->setModel(Rtmp.tri(ascending)); // Update table view with sorted data
    ascending = !ascending; // Toggle the sort order for next click
}

void gestion__reservation::on_pushButton_4_clicked()
{
    // Get the ID entered by the user
    int id_r = ui->lineEdit_ID->text().toInt();

    // Search for the reservation with the specified ID
    QSqlQueryModel *model = Rtmp.rechercheParID(id_r);

    // Check if a reservation was found
    if (model->rowCount() > 0) {
        ui->tableView->setModel(model);  // Show the result in the table view
    } else {
        QMessageBox::information(nullptr, QObject::tr("Not Found"),
                    QObject::tr("No reservation found with the specified ID.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
    }
}

void gestion__reservation::on_pushButton_Stats_clicked()
{
    // Clear the existing chart from the graphicsView
    if (ui->graphicsView->scene() != nullptr) {
        ui->graphicsView->scene()->clear();
    } else {
        ui->graphicsView->setScene(new QGraphicsScene(this));
    }

    // Get statistics from Reservation based on PRIX
    QMap<QString, double> stats = Rtmp.getStatistics();

    // Create a new bar set and fill it with data
    QBarSet *set = new QBarSet("Prix des Réservations");
    *set << stats["total"] << stats["average"] << stats["min"] << stats["max"];

    // Create a series and add the set to it
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Create a chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des Prix des Réservations");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Set up the X-axis categories
    QStringList categories;
    categories << "Total" << "Moyenne" << "Minimum" << "Maximum";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Set the Y-axis range based on statistics
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, stats["total"] * 1.1);  // Slightly higher than total for visibility
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a chart view and set it to display the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(521, 401);  // Set the size of the chart view to match graphicsView

    // Add the chart view to the scene
    ui->graphicsView->scene()->addWidget(chartView);

    // Fit the chart to the graphicsView's size
    chartView->chart()->resize(521, 401);  // Fit chart within graphicsView

    // Remove chart margins for better fit
    chart->setMargins(QMargins(0, 0, 0, 0));  // Remove any extra margins
    chart->legend()->setAlignment(Qt::AlignBottom);  // Adjust legend position if needed
}


void gestion__reservation::on_pushButton_5_clicked()
{
    // Open dialog to select save location
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer sous", "C:/Users/YourUsername/Documents/reservations_list.pdf", "PDF Files (*.pdf)");

    if (filePath.isEmpty()) {
        return;  // Return if no file path is selected
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);
    pdfWriter.setResolution(300);  // High resolution for clarity

    QPainter painter(&pdfWriter);
    painter.setFont(QFont("Arial", 10));

    // Margins and offsets
    int xOffset = 30, yOffset = 100;
    int rowHeight = 40;
    int columnWidthID = 100;
    int columnWidthPrix = 100;
    int columnWidthDateRes = 120;
    int columnWidthDebutSejour = 150;
    int columnWidthFinSejour = 150;
    int columnWidthChambre = 100;

    // Add a title with the current date
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    painter.drawText(xOffset, yOffset - 50, "Liste des Réservations - " + currentDate);

    // Draw column headers
    painter.drawText(xOffset, yOffset, "ID");
    painter.drawText(xOffset + columnWidthID, yOffset, "Prix");
    painter.drawText(xOffset + columnWidthID + columnWidthPrix, yOffset, "Date Res");
    painter.drawText(xOffset + columnWidthID + columnWidthPrix + columnWidthDateRes, yOffset, "Début Séjour");
    painter.drawText(xOffset + columnWidthID + columnWidthPrix + columnWidthDateRes + columnWidthDebutSejour, yOffset, "Fin Séjour");
    painter.drawText(xOffset + columnWidthID + columnWidthPrix + columnWidthDateRes + columnWidthDebutSejour + columnWidthFinSejour, yOffset, "Chambre");

    // Draw a separating line below the headers
    yOffset += rowHeight;
    painter.drawLine(xOffset, yOffset, xOffset + columnWidthID + columnWidthPrix + columnWidthDateRes + columnWidthDebutSejour + columnWidthFinSejour + columnWidthChambre, yOffset);
    yOffset += rowHeight;

    // Get reservation data from the model
    QSqlQueryModel *model = Rtmp.afficher();  // Replace with your afficher function
    int rowCount = model->rowCount();

    // Loop through rows and add reservation data
    for (int row = 0; row < rowCount; ++row) {
        painter.drawText(xOffset, yOffset, model->data(model->index(row, 0)).toString());  // ID
        painter.drawText(xOffset + columnWidthID, yOffset, model->data(model->index(row, 1)).toString());  // Prix
        painter.drawText(xOffset + columnWidthID + columnWidthPrix, yOffset, model->data(model->index(row, 2)).toString());  // Date Res
        painter.drawText(xOffset + columnWidthID + columnWidthPrix + columnWidthDateRes, yOffset, model->data(model->index(row, 3)).toString());  // Début Séjour
        painter.drawText(xOffset + columnWidthID + columnWidthPrix + columnWidthDateRes + columnWidthDebutSejour, yOffset, model->data(model->index(row, 4)).toString());  // Fin Séjour
        painter.drawText(xOffset + columnWidthID + columnWidthPrix + columnWidthDateRes + columnWidthDebutSejour + columnWidthFinSejour, yOffset, model->data(model->index(row, 5)).toString());  // Chambre

        yOffset += rowHeight;

        // Start a new page if the content exceeds the page height
        if (yOffset > pdfWriter.height() - 100) {
            pdfWriter.newPage();
            yOffset = 100;  // Reset position for the new page
        }
    }

    painter.end();
    QMessageBox::information(this, "Fichier Sauvegardé", "PDF sauvegardé à : " + filePath);
}


// Slot for historique functionality
void gestion__reservation::showHistorique() {
    // Créer un modèle pour tableView_2
    QSqlQueryModel *model = new QSqlQueryModel();

    // Créer un objet QSqlQuery pour exécuter la requête
    QSqlQuery query;
    query.prepare("SELECT ID_R, CHAMBRE, PRIX, DATE_RES, DEBUT_SEJOUR, FIN_SEJOUR FROM RESERVATIONS");

    // Exécuter la requête
    if (!query.exec()) {
        // Si la requête échoue, afficher l'erreur
        qDebug() << "Erreur de requête : " << query.lastError().text();
    } else {
        // Si la requête réussit, lier le modèle avec la requête
        model->setQuery(query);
    }

    // Définir les en-têtes de la table
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_R"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CHAMBRE"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("PRIX"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("DATE_RES"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("DEBUT_SEJOUR"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("FIN_SEJOUR"));

    // Vérifier si le modèle contient des données
    if (model->rowCount() > 0) {
        ui->tableView_2->setModel(model);  // Utiliser ui->tableView_2
        ui->tableView_2->resizeColumnsToContents(); // Ajuster la taille des colonnes
        ui->tableView_2->setAlternatingRowColors(true); // Activer les couleurs alternées des lignes
    } else {
        // Afficher un message si aucune donnée n'est trouvée
        QMessageBox::information(this, "Historique", "Aucune donnée disponible.");
    }
}


void gestion__reservation::updateHistorique(const QString &action, const QString &details)
{
    // For now, we'll just print the action and details to the console
    qDebug() << "Action: " << action << ", Details: " << details;

    // Alternatively, you can log it to a file or update a table view, etc.
    // Example of logging to a file:
    // QFile logFile("historique.txt");
    // if (logFile.open(QIODevice::Append | QIODevice::Text)) {
    //     QTextStream out(&logFile);
    //     out << action << ": " << details << "\n";
    // }
}
void gestion__reservation::on_pushButton_6_clicked()
{
    int id_r = ui->lineEdit_ID->text().toInt();
    int prix = ui->lineEdit_Prix->text().toInt();
    int date_res = ui->lineEdit_DateRes->text().toInt();
    int debut_sejour = ui->lineEdit_DebutSejour->text().toInt();
    int fin_sejour = ui->lineEdit_FinSejour->text().toInt();
    QString chambre = ui->lineEdit_Chambre->text();

    Reservation R(id_r, prix, date_res, debut_sejour, fin_sejour, chambre);
    bool test = R.ajouter();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Ajout réussi"),
                    QObject::tr("La réservation a été ajoutée avec succès.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->tableView->setModel(Rtmp.afficher()); // Refresh reservation display
        updateHistorique("Add", "ID: " + QString::number(id_r) +  ", Chambre: " + chambre);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                 QObject::tr("L'ajout de la réservation a échoué.\n"
                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}


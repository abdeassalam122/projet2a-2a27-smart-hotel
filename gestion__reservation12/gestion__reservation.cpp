#include "gestion__reservation.h"
#include "ui_gestion__reservation.h"
#include "reservation.h"
#include <QMessageBox>
#include <QSqlQuery>         // Pour les requêtes SQL
#include <QSqlQueryModel>    // Pour les modèles de résultats SQL
#include <QtCharts/QChartView>       // Pour afficher les graphiques
#include <QtCharts/QChart>           // Pour créer les graphiques
#include <QtCharts/QBarSeries>       // Pour les séries de barres
#include <QtCharts/QBarSet>          // Pour les ensembles de barres
#include <QtCharts/QBarCategoryAxis> // Pour les catégories de l'axe X
#include <QtCharts/QValueAxis>       // Pour les valeurs de l'axe Y
#include <QGraphicsScene>            // Pour gérer la scène graphique
#include <QGraphicsView>             // Pour afficher la scène graphique
#include "dialog.h"
#include <QDebug>
#include <QFileDialog>  // Ajoutez cette ligne pour inclure QFileDialog
#include <QTextDocument>
#include "logviewer.h"  // Inclure le fichier d'en-tête de LogViewer
#include <QTextCharFormat>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

using namespace QtCharts;

gestion__reservation::gestion__reservation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gestion__reservation)
{
    ui->setupUi(this);

    // Configuration de la ComboBox pour les types de chambres
    ui->comboBox_Chambre->addItem("Chambre simple");
    ui->comboBox_Chambre->addItem("Chambre double");
    ui->comboBox_Chambre->addItem("Suite");
    ui->comboBox_Chambre->addItem("Chambre avec balcon");

    // Afficher les réservations dans la tableView au lancement
    ui->tableView->setModel(Rtmp.afficher());
    // Connecter le bouton pushButtonSend au slot pour afficher la fenêtre Dialog
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &gestion__reservation::on_pushButtonSend_clicked);
    connect(ui->pushButtonExportPDF, &QPushButton::clicked, this, &gestion__reservation::exportDataToPDF);
    connect(ui->pushButton_ViewLogs_2, &QPushButton::clicked, this, &gestion__reservation::on_pushButton_ViewLogs_2_clicked);
    // Connexion du signal clicked(QDate) à la fonction onDateClicked
    connect(ui->calendarWidget, &QCalendarWidget::clicked, this, &gestion__reservation::onDateClicked);
    // Cacher le frame qui affichera les informations au début
    ui->frame_Chambres->hide();

}

gestion__reservation::~gestion__reservation()
{
    // Libérer la mémoire associée à l'interface utilisateur
    delete ui;
}

// Méthode pour ajouter une réservation
void gestion__reservation::on_pushButton_clicked()
{
    // Récupérer les données saisies par l'utilisateur
    int id_r = ui->lineEdit_ID->text().toInt();
    int prix = ui->lineEdit_Prix->text().toInt();
    QDate date_res = ui->dateEdit_DateRes->date();  // Utiliser QDate directement
    QDate debut_sejour = ui->dateEdit_DebutSejour->date();  // Utiliser QDate directement
    QDate fin_sejour = ui->dateEdit_FinSejour->date();  // Utiliser QDate directement
    QString chambre = ui->comboBox_Chambre->currentText();

    // Créer un objet Reservation avec les données récupérées
    Reservation R(id_r, prix, date_res, debut_sejour, fin_sejour, chambre);

    // Ajouter la réservation et vérifier si l'ajout a réussi
    bool test = R.ajouter();
    if (test) {
        // Enregistrer l'événement dans le fichier log
        LogViewer::writeLog("Ajout de la réservation ID: ", QString::number(id_r));

        // Afficher un message de succès et rafraîchir la tableView
        QMessageBox::information(nullptr, QObject::tr("Ajout réussi"),
                    QObject::tr("La réservation a été ajoutée avec succès.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->tableView->setModel(Rtmp.afficher()); // Mise à jour de l'affichage
    } else {
        // Afficher un message d'erreur si l'ajout a échoué
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                 QObject::tr("L'ajout de la réservation a échoué.\n"
                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}



// Méthode pour supprimer une réservation
void gestion__reservation::on_pushButton_3_clicked()
{
    // Récupérer l'ID de la réservation à supprimer
    int id_r = ui->lineEdit_ID->text().toInt();

    // Appeler la méthode de suppression
    bool test = Rtmp.supprimer(id_r);
    if (test) {
        // Enregistrer l'événement dans le fichier log
        LogViewer::writeLog("Suppression de la réservation ID: ", QString::number(id_r));

        // Afficher un message de succès et rafraîchir la tableView
        QMessageBox::information(nullptr, QObject::tr("Suppression réussie"),
                    QObject::tr("La réservation a été supprimée avec succès.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->tableView->setModel(Rtmp.afficher()); // Mise à jour de l'affichage
    } else {
        // Afficher un message d'erreur si la suppression a échoué
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                    QObject::tr("La suppression de la réservation a échoué.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}


// Méthode pour modifier une réservation
void gestion__reservation::on_pushButton_2_clicked()
{
    // Récupérer les données saisies par l'utilisateur
    int id_r = ui->lineEdit_ID->text().toInt();
    int prix = ui->lineEdit_Prix->text().toInt();
    QDate date_res = ui->dateEdit_DateRes->date();  // Utiliser QDate directement
    QDate debut_sejour = ui->dateEdit_DebutSejour->date();  // Utiliser QDate directement
    QDate fin_sejour = ui->dateEdit_FinSejour->date();  // Utiliser QDate directement
    QString chambre = ui->comboBox_Chambre->currentText();

    // Créer un objet Reservation avec les données récupérées
    Reservation R(id_r, prix, date_res, debut_sejour, fin_sejour, chambre);

    // Modifier la réservation et vérifier si la modification a réussi
    bool test = R.modifier(id_r);  // Utilisez id_r pour la modification de la réservation

    if (test) {
        // Enregistrer l'événement dans le fichier log
        LogViewer::writeLog("Modification de la réservation ID: ", QString::number(id_r));

        // Afficher un message de succès et rafraîchir la tableView
        QMessageBox::information(nullptr, QObject::tr("Modification réussie"),
                    QObject::tr("La réservation a été modifiée avec succès.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
        ui->tableView->setModel(Rtmp.afficher()); // Mise à jour de l'affichage
    } else {
        // Afficher un message d'erreur si la modification a échoué
        QMessageBox::critical(nullptr, QObject::tr("Erreur"),
                 QObject::tr("La modification de la réservation a échoué.\n"
                             "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}



// Méthode pour trier les réservations
void gestion__reservation::on_pushButton_T_clicked()
{
    // Alternance de l'ordre de tri (ascendant/descendant)
    static bool ascending = true;
    ui->tableView->setModel(Rtmp.tri(ascending)); // Mise à jour des données triées
    ascending = !ascending; // Inverser l'ordre pour le prochain tri
}

// Méthode pour rechercher une réservation par ID
void gestion__reservation::on_pushButton_4_clicked()
{
    // Récupérer l'ID saisi par l'utilisateur
    int id_r = ui->lineEdit_ID->text().toInt();

    // Effectuer une recherche dans la base de données
    QSqlQueryModel *model = Rtmp.rechercheParID(id_r);

    if (model->rowCount() > 0) {
        // Afficher les résultats dans la tableView
        ui->tableView->setModel(model);
    } else {
        // Afficher un message si aucun résultat n'a été trouvé
        QMessageBox::information(nullptr, QObject::tr("Non trouvé"),
                    QObject::tr("Aucune réservation trouvée avec cet ID.\n"
                                "Cliquez sur Annuler pour quitter."), QMessageBox::Cancel);
    }
}

// Méthode pour afficher les statistiques
void gestion__reservation::showReservationPieChart()
{
    // Requête SQL pour obtenir les statistiques du nombre de réservations par type de chambre
    QSqlQuery query;
    query.prepare("SELECT CHAMBRE, COUNT(*) FROM RESERVATIONS GROUP BY CHAMBRE");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    // Créer une série de type pie pour représenter le nombre de réservations pour chaque type de chambre
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
    int totalReservations = 0;

    while (query.next()) {
        QString chambreType = query.value(0).toString();
        int count = query.value(1).toInt();

        totalReservations += count;

        // Ajouter chaque type de chambre avec son nombre de réservations comme une tranche (slice)
        series->append(chambreType, count);
    }

    // Ajouter des labels à chaque tranche (slice) pour afficher les détails
    for (QtCharts::QPieSlice *slice : series->slices()) {
        slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));
    }

    // Créer le graphique
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Nombre de réservations par type de chambre : Total = %1").arg(totalReservations));
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    // Ajuster la taille du graphique et supprimer les marges
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Créer la vue pour afficher le graphique
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(525, 405);

    // Vider la scène existante et ajouter le graphique à la nouvelle scène
    if (ui->graphicsView->scene() != nullptr) {
        ui->graphicsView->scene()->clear();
    } else {
        ui->graphicsView->setScene(new QGraphicsScene(this));
    }
    ui->graphicsView->scene()->addWidget(chartView);

    // S'assurer que le graphique s'ajuste bien dans la vue graphicsView_2
    chartView->setFixedSize(525, 405);
}

// Cette fonction sera appelée lorsque vous cliquerez sur le bouton pour afficher les statistiques
void gestion__reservation::on_pushButton_Stats_clicked()
{
    showReservationPieChart();  // Appeler la fonction pour afficher les statistiques
}



// Slot qui sera appelé lors du clic sur le bouton pushButtonSend
void gestion__reservation::on_pushButtonSend_clicked()
{
    qDebug() << "Affichage du Dialog";

    // Si l'instance de Dialog existe déjà, on ne la recrée pas
    if (dialogInstance == nullptr) {
        dialogInstance = new Dialog(this);  // Créer une nouvelle instance de Dialog
        dialogInstance->setModal(true);  // Utiliser le mode modal pour bloquer l'interface principale
        dialogInstance->show();  // Afficher le Dialog
    } else {
        qDebug() << "Le Dialog est déjà ouvert.";  // Si le Dialog est déjà ouvert
    }
}
void gestion__reservation::exportDataToPDF() {
    // Ouvrir une boîte de dialogue pour choisir l'emplacement de sauvegarde
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer en tant que PDF", "", "*.pdf");
    if (!filePath.isEmpty()) {
        // Vérifier que le chemin se termine par .pdf
        if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
            filePath.append(".pdf");
        }

        // Appeler la méthode exportToPDF de l'objet Reservation
        if (Rtmp.exportToPDF(filePath)) {
            QMessageBox::information(this, QObject::tr("Export PDF"),
                                     QObject::tr("Données exportées avec succès en PDF."));
        } else {
            QMessageBox::critical(this, QObject::tr("Erreur d'export PDF"),
                                  QObject::tr("Échec de l'exportation des données en PDF."));
        }
    }
}
void gestion__reservation::on_pushButtonExportPDF_clicked()
{
    qDebug() << "Export PDF clicked!";

    // Ouvrir une boîte de dialogue pour choisir l'emplacement de sauvegarde
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer en tant que PDF", "", "*.pdf");
    if (!filePath.isEmpty()) {
        // Vérifier que le chemin se termine par .pdf
        if (!filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
            filePath.append(".pdf");
        }

        // Appeler la méthode exportToPDF de l'objet Reservation
        if (Rtmp.exportToPDF(filePath)) {
            QMessageBox::information(this, QObject::tr("Export PDF"),
                                     QObject::tr("Données exportées avec succès en PDF."));
        } else {
            QMessageBox::critical(this, QObject::tr("Erreur d'export PDF"),
                                  QObject::tr("Échec de l'exportation des données en PDF."));
        }
    }
}
void gestion__reservation::on_pushButton_ViewLogs_2_clicked()
{
    LogViewer *logViewer = new LogViewer(this);
    logViewer->exec();  // Affiche la fenêtre LogViewer modale
}
void gestion__reservation::on_pushButton_calendar_clicked()
{
    // Vérifier si un ID de réservation a été saisi
    int reservationID = ui->lineEdit_ID->text().toInt();
    if (reservationID <= 0) {
        QMessageBox::warning(this, "ID invalide", "Veuillez entrer un ID de réservation valide.");
        return;
    }

    // Appeler la fonction pour colorer les dates
    highlightReservationDates(reservationID);

    // Afficher le calendrier
    ui->calendarWidget->show();
}

void gestion__reservation::highlightReservationDates(int reservationID)
{
    // Requête pour récupérer les dates de début et de fin de la réservation
    QSqlQuery query;
    query.prepare("SELECT DEBUT_SEJOUR, FIN_SEJOUR FROM RESERVATIONS WHERE ID_R = :id");
    query.bindValue(":id", reservationID);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "Réservation introuvable", "Aucune réservation trouvée pour cet ID.");
        return;
    }

    QDate startDate = query.value("DEBUT_SEJOUR").toDate();
    QDate endDate = query.value("FIN_SEJOUR").toDate();

    if (!startDate.isValid() || !endDate.isValid() || startDate > endDate) {
        QMessageBox::warning(this, "Dates invalides", "Les dates récupérées sont invalides.");
        return;
    }

    // Format de coloration rouge
    QTextCharFormat redFormat;
    redFormat.setBackground(Qt::red);

    // Boucle pour colorer les dates de la plage
    QDate currentDate = startDate;
    while (currentDate <= endDate) {
        ui->calendarWidget->setDateTextFormat(currentDate, redFormat);
        currentDate = currentDate.addDays(1); // Passer au jour suivant
    }

    QMessageBox::information(this, "Calendrier", "Les dates de la réservation sont colorées en rouge.");
}
void gestion__reservation::onDateClicked(const QDate &date) {
    qDebug() << "Date cliquée :" << date.toString("dd/MM/yyyy");

    // Exécuter une requête SQL pour compter les réservations par type de chambre pour la date cliquée
    QSqlQuery query;
    query.prepare("SELECT CHAMBRE, COUNT(*) AS nombre_reservations "
                  "FROM RESERVATIONS "
                  "WHERE :selectedDate BETWEEN DEBUT_SEJOUR AND FIN_SEJOUR "
                  "GROUP BY CHAMBRE");
    query.bindValue(":selectedDate", date);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête :" << query.lastError().text();
        return;
    }

    QString details = "Date sélectionnée : " + date.toString("dd/MM/yyyy") + "\n\n";

    while (query.next()) {
        QString chambreType = query.value("CHAMBRE").toString();
        int count = query.value("nombre_reservations").toInt();
        details += chambreType + " : " + QString::number(count) + " réservation(s)\n";
    }

    // Afficher les informations dans le label de frame_Chambres
    ui->label_Chambres->setText(details);
    ui->frame_Chambres->show(); // Afficher le frame avec les détails
}


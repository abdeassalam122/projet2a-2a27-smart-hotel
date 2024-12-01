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
void gestion__reservation::on_pushButton_Stats_clicked()
{
    // Vider la scène existante dans la graphicsView
    if (ui->graphicsView->scene() != nullptr) {
        ui->graphicsView->scene()->clear();
    } else {
        ui->graphicsView->setScene(new QGraphicsScene(this));
    }

    // Obtenir les statistiques de la classe Reservation
    QMap<QString, double> stats = Rtmp.getStatistics();

    // Créer un ensemble de barres et y ajouter les statistiques
    QBarSet *set = new QBarSet("Prix des Réservations");
    *set << stats["total"] << stats["average"] << stats["min"] << stats["max"];

    // Créer une série et y ajouter l'ensemble de barres
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Créer un graphique et y ajouter la série
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des Prix des Réservations");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Configurer les catégories de l'axe X
    QStringList categories;
    categories << "Total" << "Moyenne" << "Minimum" << "Maximum";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Configurer l'axe Y avec une plage adaptée
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, stats["total"] * 1.1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Ajouter le graphique à la scène et ajuster sa taille
    QChartView *chartView = new QChartView(chart);
    chart;
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


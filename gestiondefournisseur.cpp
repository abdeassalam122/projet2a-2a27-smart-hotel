#include "gestiondefournisseur.h"
#include "ui_gestiondefournisseur.h"
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QMessageBox>
#include <QDateTime>  // Pour obtenir la date et l'heure actuelles
#include <QFileDialog>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QTextDocument>
#include <QTextCursor>
#include <QFile>
#include <QTextStream>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QSqlError>  // Include for lastError()
#include <QMessageBox>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>
#include <QGraphicsScene>
#include <QSqlQuery>
#include <QSqlError>
#include "arduino.h"
#include "connection.h"
using namespace QtCharts;

gestiondefournisseur::gestiondefournisseur(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gestiondefournisseur),
    ordreAscendant(true),
    arduino(new Arduino()) // Proper initialization of Arduino pointer
{
    ui->setupUi(this);
    ui->tableView_2->setModel(Ftmp.afficher());
    if (arduino->connectToArduino() == 0) {
        qDebug() << "Arduino connected successfully!";
    } else {
        qDebug() << "Failed to connect to Arduino.";
    }
    if (!db.open()) {
        qDebug() << "Failed to open the database!";
        return;
    }

    qDebug() << "Database connected successfully!";
}


gestiondefournisseur::~gestiondefournisseur()
{
    delete ui;
    if (arduino) {
            delete arduino; // Properly clean up Arduino object
        }
    // Close the database connection if open
    if (db.isOpen()) {
            db.close();
        }
        QSqlDatabase::removeDatabase("reservations_connection");
}

void gestiondefournisseur::on_pushButton_ajouter_2_clicked()
{
    int id = ui->lineEdit_ID_2->text().toInt();
    QString Nom_F = ui->lineEdit_nom_3->text();
    QString Adresse = ui->lineEdit_adresse_3->text();
    QString Email = ui->lineEdit_email_3->text();
    QString Telephone = ui->lineEdit_telephone_3->text();
    QString Secteur = ui->lineEdit_secteur_3->text();

    Fournisseur F(id, Nom_F, Adresse, Email, Telephone, Secteur);

    bool test = F.ajouter();

    if (test)
    {
        ui->tableView_2->setModel(Ftmp.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Ajout effectué\n"), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Ajout non effectué.\n"), QMessageBox::Cancel);
    }
}

void gestiondefournisseur::on_pushButton_supprimer_2_clicked()
{
    int id = ui->lineEdit_ID_2->text().toInt();
    bool test = Ftmp.supprimer(id);

    if (test)
    {
        ui->tableView_2->setModel(Ftmp.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Suppression effectuée\n"), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Suppression non effectuée.\n"), QMessageBox::Cancel);
    }
}
void gestiondefournisseur::on_pushButton_modifier_2_clicked()
{
    int id = ui->lineEdit_ID_2->text().toInt();
    QString Nom_F = ui->lineEdit_nom_3->text();
    QString Adresse = ui->lineEdit_adresse_3->text();
    QString Email = ui->lineEdit_email_3->text();
    QString Telephone = ui->lineEdit_telephone_3->text();
    QString Secteur = ui->lineEdit_secteur_3->text();

    Fournisseur F(id, Nom_F, Adresse, Email, Telephone, Secteur);

    if (F.modifier())
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                    QObject::tr("Modification effectuée\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        ui->tableView_2->setModel(Ftmp.afficher());  // Rafraîchir la vue de table
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                    QObject::tr("Modification non effectuée.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
    }
}


void gestiondefournisseur::on_pushButton_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    if (ordreAscendant)
    {
        model->setQuery("SELECT * FROM FOURNISSEUR ORDER BY ID_F ASC"); // Tri en ordre croissant
    }
    else
    {
        model->setQuery("SELECT * FROM FOURNISSEUR ORDER BY ID_F DESC"); // Tri en ordre décroissant
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID_F"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("NOM"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ADRESSE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("EMAIL"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("TELEPHONE"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("SECTEUR_ACTIVITE"));

    ui->tableView_2->setModel(model); // Met à jour la vue de table avec les données triées

    ordreAscendant = !ordreAscendant; // Inverse l'ordre de tri pour le prochain clic
}



void gestiondefournisseur::on_pushButton_2_clicked()
{
    // Get the input from the search field for "secteur d'activité"
    QString secteur = ui->lineEdit_secteur_3->text();

    // Set the model of the table view to display the search results
    ui->tableView_2->setModel(Ftmp.rechercherParSecteur(secteur));
}

void gestiondefournisseur::on_pushButton_3_clicked()
{
    // Clear the existing chart from the graphicsView
    if (ui->graphicsView->scene() != nullptr) {
        ui->graphicsView->scene()->clear();
    } else {
        ui->graphicsView->setScene(new QGraphicsScene(this));
    }

    // Get statistics from Fournisseur (secteurs d'activité)
    QMap<QString, QMap<QString, double>> sectorStats;

    // Exemple de données, à remplacer par les statistiques réelles de Ftmp
    sectorStats["Équipements et technologies"] = {{"total", 200}, {"average", 50}, {"min", 10}, {"max", 100}};
    sectorStats["Alimentation et boissons"] = {{"total", 150}, {"average", 40}, {"min", 5}, {"max", 80}};
    sectorStats["Sécurité et surveillance"] = {{"total", 180}, {"average", 45}, {"min", 20}, {"max", 90}};
    sectorStats["Bien-être et spa"] = {{"total", 220}, {"average", 55}, {"min", 15}, {"max", 110}};

    QBarSeries *series = new QBarSeries();

    // Iterate through each sector and create a bar set
    for (auto it = sectorStats.begin(); it != sectorStats.end(); ++it) {
        QBarSet *set = new QBarSet(it.key());  // Sector name as the label
        *set << it.value()["total"] << it.value()["average"] << it.value()["min"] << it.value()["max"];
        series->append(set);
    }

    // Create a chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des Fournisseurs par Secteur");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Set up the X-axis categories
    QStringList categories;
    categories << "Total" << "Moyenne" << "Minimum" << "Maximum";

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Set the Y-axis range (adjustable based on data scale)
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 120);  // Adjust the range based on the maximum value in your data
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a chart view and set it to display the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(521, 401);  // Size of the chart view

    // Add the chart view to the scene
    ui->graphicsView->scene()->addWidget(chartView);
}


void gestiondefournisseur::on_pushButton_4_clicked()
{
    // Affichage de la boîte de dialogue pour choisir le chemin de sauvegarde
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer sous", "C:/Users/lenovo/OneDrive/Desktop/Qt Creator/Gestion_De_Fournisseur/fournisseurs_list.pdf", "PDF Files (*.pdf)");

    if (filePath.isEmpty()) {
        return;  // Annuler si aucun chemin n'est choisi
    }

    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);
    pdfWriter.setResolution(300);  // Haute résolution pour une meilleure clarté

    QPainter painter(&pdfWriter);
    painter.setFont(QFont("Arial", 10));

    // Marges et espacements ajustés
    int xOffset = 30, yOffset = 150;
    int rowHeight = 40;
    int columnWidthID = 170;
    int columnWidthNom = 250;
    int columnWidthAdresse = 400;
    int columnWidthEmail = 200;
    int columnWidthTel = 150;
    int columnWidthSecteur = 200;

    // Ajouter un titre avec la date actuelle
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    painter.drawText(xOffset, yOffset - 50, "Liste des Fournisseurs - " + currentDate);

    // Dessiner les entêtes avec plus d'espacement
    painter.drawText(xOffset, yOffset, "ID");
    painter.drawText(xOffset + columnWidthID, yOffset, "Nom");
    painter.drawText(xOffset + columnWidthID + columnWidthNom, yOffset, "Adresse");
    painter.drawText(xOffset + columnWidthID + columnWidthNom + columnWidthAdresse, yOffset, "Email");
    painter.drawText(xOffset + columnWidthID + columnWidthNom + columnWidthAdresse + columnWidthEmail, yOffset, "Téléphone");
    painter.drawText(xOffset + columnWidthID + columnWidthNom + columnWidthAdresse + columnWidthEmail + columnWidthTel, yOffset, "Secteur");

    // Dessiner une ligne de séparation sous les entêtes
    yOffset += rowHeight;
    painter.drawLine(xOffset, yOffset, xOffset + columnWidthID + columnWidthNom + columnWidthAdresse + columnWidthEmail + columnWidthTel + columnWidthSecteur, yOffset);
    yOffset += rowHeight;

    // Récupérer les données des fournisseurs à partir du modèle (ex : Ftmp)
    QSqlQueryModel *model = Ftmp.afficher();  // Vous devez avoir une fonction afficher() pour récupérer la liste des fournisseurs
    int rowCount = model->rowCount();

    // Itérer à travers chaque ligne et ajouter les données des fournisseurs
    for (int row = 0; row < rowCount; ++row) {
        painter.drawText(xOffset, yOffset, model->data(model->index(row, 0)).toString());  // ID
        painter.drawText(xOffset + columnWidthID, yOffset, model->data(model->index(row, 1)).toString());  // Nom
        painter.drawText(xOffset + columnWidthID + columnWidthNom, yOffset, model->data(model->index(row, 2)).toString());  // Adresse
        painter.drawText(xOffset + columnWidthID + columnWidthNom + columnWidthAdresse, yOffset, model->data(model->index(row, 3)).toString());  // Email
        painter.drawText(xOffset + columnWidthID + columnWidthNom + columnWidthAdresse + columnWidthEmail, yOffset, model->data(model->index(row, 4)).toString());  // Téléphone
        painter.drawText(xOffset + columnWidthID + columnWidthNom + columnWidthAdresse + columnWidthEmail + columnWidthTel, yOffset, model->data(model->index(row, 5)).toString());  // Secteur

        yOffset += rowHeight + 10;

        // Si la position dépasse la hauteur de la page, ajouter une nouvelle page
        if (yOffset > pdfWriter.height() - 100) {
            pdfWriter.newPage();
            yOffset = 100;  // Réinitialiser la position pour la nouvelle page
        }
    }

    painter.end();
    QMessageBox::information(this, "Fichier Sauvegardé", "PDF sauvegardé à : " + filePath);
}
#include <QTcpSocket>
#include <QTextStream>
#include <QMessageBox>

// Fonction pour envoyer un e-mail
void gestiondefournisseur::envoyerEmailConfirmation(const QString &destinataire, const QString &sujet, const QString &message)
{
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost("smtp.votre_serveur_smtp.com", 587);  // Remplacez par l'adresse de votre serveur SMTP et le port correct

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Erreur", "Connexion au serveur SMTP échouée.");
        return;
    }

    QTextStream stream(socket);
    stream.setAutoDetectUnicode(true);

    // Envoyer les commandes SMTP
    stream << "EHLO localhost\r\n";
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    stream << "AUTH LOGIN\r\n";
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    // Envoyer le nom d'utilisateur et le mot de passe encodés en base64
    stream << "VOTRE_NOM_UTILISATEUR_BASE64\r\n"; // Remplacez par votre nom d'utilisateur encodé
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    stream << "VOTRE_MOT_DE_PASSE_BASE64\r\n"; // Remplacez par votre mot de passe encodé
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    // Préparer l'e-mail
    stream << "MAIL FROM: <votre_email@example.com>\r\n"; // Adresse e-mail de l'expéditeur
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    stream << "RCPT TO: <" << destinataire << ">\r\n"; // Adresse e-mail du destinataire
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    stream << "DATA\r\n";
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    // Contenu de l'e-mail
    stream << "Subject: " << sujet << "\r\n";
    stream << "\r\n";
    stream << message << "\r\n";
    stream << ".\r\n"; // Terminer le message par une ligne contenant uniquement un point
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    stream << "QUIT\r\n";
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    QMessageBox::information(this, "Succès", "E-mail de confirmation envoyé avec succès !");
    socket->disconnectFromHost();
}

// Exemple d'utilisation
void gestiondefournisseur::on_pushButton_5_clicked()
{
    // Récupérer les informations du fournisseur depuis les champs de l'interface
    QString destinataire = ui->lineEdit_email_3->text();
    QString sujet = "Confirmation de votre inscription";
    QString message = "Bonjour,\n\nVotre inscription a été confirmée avec succès. Merci de faire partie de notre réseau.\n\nCordialement,\nVotre Entreprise";

    envoyerEmailConfirmation(destinataire, sujet, message);
}

void gestiondefournisseur::on_pushButton_6_clicked()
{
    // Affichage de la boîte de dialogue pour choisir le chemin de sauvegarde
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer sous", "C:/Users/merie/Documents/gestion_des_fournisseurs/fournisseurs_list.rtf", "Rich Text Format (*.rtf)");

    if (filePath.isEmpty()) {
        return;  // Annuler si aucun chemin n'est choisi
    }

    // Création du document QTextDocument
    QTextDocument document;
    QTextCursor cursor(&document);

    // Ajouter un titre avec la date actuelle
    QString currentDate = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    cursor.insertText("Liste des Fournisseurs - " + currentDate + "\n\n");

    // Ajouter les entêtes
    cursor.insertText("ID_F\tNOM\n");
    cursor.insertText("----------------------------------------------------\n");

    // Récupérer les données des fournisseurs à partir du modèle (ex : Ftmp)
    QSqlQueryModel *model = Ftmp.afficher();
    int rowCount = model->rowCount();

    // Itérer à travers chaque ligne et ajouter les données des fournisseurs (ID et Nom)
    for (int row = 0; row < rowCount; ++row) {
        QString id = model->data(model->index(row, 0)).toString();
        QString nom = model->data(model->index(row, 1)).toString();
        cursor.insertText(id + "\t" + nom + "\n");
    }

    // Enregistrer le document en tant que fichier RTF
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream out(&file);
        out << document.toHtml();  // Utiliser le format HTML compatible RTF
        file.close();
    }

    QMessageBox::information(this, "Fichier Sauvegardé", "Fichier RTF sauvegardé à : " + filePath);
}



void gestiondefournisseur::showSectorPieChart()
{
    // Query the database for sector data
    QSqlQuery query;
    query.prepare("SELECT secteur_activite, COUNT(*) FROM fournisseur GROUP BY secteur_activite");

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    // Create a pie series to represent the number of suppliers in each sector
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
    int totalSuppliers = 0;

    while (query.next()) {
        QString sector = query.value(0).toString();
        int count = query.value(1).toInt();

        totalSuppliers += count;

        // Add each sector's count as a slice
        series->append(sector, count);
    }

    // Add labels to each slice
    for (QtCharts::QPieSlice *slice : series->slices()) {
        slice->setLabel(QString("%1: %2").arg(slice->label()).arg(slice->value()));
    }

    // Create the chart
    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Number of Suppliers by Sector: Total = %1").arg(totalSuppliers));
    chart->setAnimationOptions(QtCharts::QChart::SeriesAnimations);

    // Adjust chart size and remove margins
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create the chart view
    QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(525, 405);

    // Clear the existing scene and set the chart view
    if (ui->graphicsView_2->scene() != nullptr) {
        ui->graphicsView_2->scene()->clear();
    } else {
        ui->graphicsView_2->setScene(new QGraphicsScene(this));
    }
    ui->graphicsView_2->scene()->addWidget(chartView);

    // Ensure the chart fits into the graphicsView_2
    chartView->setFixedSize(525, 405);
}

void gestiondefournisseur::on_pushButton_8_clicked()
{
    showSectorPieChart();
}
void gestiondefournisseur::on_pushButton_7_clicked()
{
    QString reservationID = ui->lineEdit_reservationID->text();

    // Validate reservation ID input
    if (reservationID.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Reservation ID cannot be empty!");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(); // Get the database instance

    // Ensure the database is open
    if (!db.isOpen()) {
        qDebug() << "Database not open! Error:" << db.lastError().text();
        QMessageBox::critical(this, "Database Error", "Database is not open!");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM RESERVATIONS WHERE ID_RESERVATION = :ID_RESERVATION");
    query.bindValue(":ID_RESERVATION", reservationID);

    // Execute the query
    if (!query.exec()) {
        qDebug() << "Query execution failed! Error:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to execute query: " + query.lastError().text());
        return;
    }

    // Check if the reservation exists
    if (!query.next()) {
        qDebug() << "No reservation found for ID:" << reservationID;
        QMessageBox::warning(this, "Not Found", "Reservation ID not found!");
        return;
    }

    // If the reservation exists, send it to the Arduino
    if (arduino->connectToArduino() == 0) {
        if (arduino->sendDataToArduino(reservationID) == 0) {
            QMessageBox::information(this, "Success", "Reservation ID sent to Arduino!");
        } else {
            qDebug() << "Failed to send data to Arduino!";
            QMessageBox::critical(this, "Error", "Failed to send data to Arduino!");
        }
        arduino->disconnectFromArduino();
    } else {
        qDebug() << "Arduino connection failed!";
        QMessageBox::critical(this, "Error", "Failed to connect to Arduino!");
    }
}

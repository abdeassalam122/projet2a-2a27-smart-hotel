#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "employe.h"
#include <QSqlQuery>
#include <QFileDialog>
#include <QSqlError>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QPrinter>
#include <QFileDialog>
#include <QFileInfo>
#include <QTextDocument>
#include <QTextStream>
#include "Qrcode.hpp"
Employe e;
using namespace  QtCharts;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->aff->setModel(e.afficher());
    ui->aff_feed_2->setModel(e.afficherEmpFeedback());
    afficherStatistiques();
    ui->gestion_employe->hide();
    ui->gestion_client->hide();
    ui->menu->show();
    ui->login->show();
    ui->tableView->setModel(clt.afficher());
     ui->histo->setModel(clt.afficher_historique0());

     if (A.connect_arduino() == 0) {

               QMessageBox::information(this, "Arduino connecté sur le port:" , A.getarduino_port_name());

           } else {
               qDebug() << "";
               QMessageBox::warning(this,  "","Échec de la connexion à l'Arduino.");

           }
           connect(A.get_serial(), &QSerialPort::readyRead, this, &MainWindow::readFromArduino);
 }



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::readFromArduino()
{
    QByteArray data = A.read_from_arduino();
    QString uid = QString::fromUtf8(data).trimmed(); // Convertir les données en chaîne
    if (!uid.isEmpty())
    {
        //qDebug() << "UID reçu:" << uid;
        if (uid != "/")
        {
            A.code += uid;
        }
        else
        {
            A.code.remove("/");
            QSqlQuery query;
            query.prepare("SELECT EMAIL,PASSWORD FROM EMPLOYE WHERE RFID_CARD_ID = :code");
            query.bindValue(":code", A.code);
            if (query.exec() && query.next())
            {
                QString email = query.value(0).toString();
                QString password = query.value(1).toString();



                        QMessageBox::information(this, "Succès", "L'UID existe  !");
                        A.code = "";
            ui->email->setText(email);
            ui->password->setText(password);
                    }
                    else
                    {
                    // UID does not exist
                        QMessageBox::warning(this, "Erreur", "L'UID n'existe pas !");
                        A.code = "";
                    }

                }
            }




}


void MainWindow::on_modifier_clicked()
{
    int id = ui->id_m->text().toInt();
    QString nom = ui->nom_2->text();
    QString prenom = ui->prenom_2->text();
    QString role = ui->role_2->currentText();
    double salaire = ui->salaire_2->text().toDouble();
QString email=ui->email->text();
    if (id <= 0 || nom.isEmpty() || prenom.isEmpty() || role.isEmpty() || salaire <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs correctement !");
        return;
    }

    Employe e(id, nom, prenom, role, salaire,email);
    if (e.modifier()) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
        ui->aff->setModel(e.afficher());
        afficherStatistiques();

    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification de l'employé !");
    }}


void MainWindow::on_recuperer_clicked() {
    int id = ui->id_m->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    Employe e;
    if (!e.verifierId(id)) {
        QMessageBox::warning(this, "Erreur", "Aucun employé trouvé avec cet ID !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT nom, prenom, role, salaire FROM employe WHERE id_e = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next()) {
        ui->nom_2->setText(query.value(0).toString());
        ui->prenom_2->setText(query.value(1).toString());
        ui->role_2->setCurrentText(query.value(2).toString());
        ui->salaire_2->setText(query.value(3).toString());
        QMessageBox::information(this, "Succès", "Données récupérées avec succès !");
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de récupérer les données !");
    }
}


void MainWindow::on_supprimer_clicked() {
    int id = ui->id_supp->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID valide !");
        return;
    }

    Employe e;
    if (!e.verifierId(id)) {
        QMessageBox::warning(this, "Erreur", "Aucun employé trouvé avec cet ID !");
        return;
    }

    if (e.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
        ui->aff->setModel(e.afficher());
        afficherStatistiques();

    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la suppression de l'employé !");
    }
}







void MainWindow::on_trie_clicked()
{
    QString critere = ui->tri->currentText();

    if (critere != "role" && critere != "salaire") {
        QMessageBox::warning(this, "Erreur", "Critère de tri invalide. Choisissez 'role' ou 'salaire'.");
        return;
    }

    QSqlQueryModel *model = e.trier(critere);
    ui->aff->setModel(model);
}


void MainWindow::on_rechercher_clicked()
{
    QString critere = ui->rech->currentText();
    QString valeur = ui->valeur->text();


    QSqlQueryModel *model = e.rechercher(critere, valeur);
    ui->aff->setModel(model);
}


void MainWindow::on_pdf_clicked()
{
    // Ouvrir une boîte de dialogue pour choisir l'emplacement du fichier PDF
    QString fichierPDF = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "*.pdf");

    if (!fichierPDF.isEmpty()) {
        if (!fichierPDF.endsWith(".pdf", Qt::CaseInsensitive)) {
            fichierPDF += ".pdf";  // Ajouter l'extension si elle n'est pas fournie
        }



        // Générer le PDF à l'emplacement choisi
        e.genererPDF(fichierPDF);

        // Informer l'utilisateur que le PDF a été généré avec succès
        QMessageBox::information(this, "Succès", "Le PDF a été généré avec succès.");
    } else {
        // Si l'utilisateur annule l'opération
        QMessageBox::warning(this, "Annulé", "La génération du PDF a été annulée.");
    }
}


void MainWindow::afficherStatistiques() {

    // Récupérer la série de statistiques (pie chart)
    QPieSeries* series = e.statistiques();

    // Créer un objet QChart pour afficher le graphique
    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des employes par role");

    // Créer un QChartView pour intégrer le QChart
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);  // Améliorer le rendu

    // Créer une QGraphicsScene et ajouter le QChartView
    QGraphicsScene* scene = new QGraphicsScene(this);
    scene->addWidget(chartView);

    // Ajouter la scène dans le QGraphicsView (stat_e)
    ui->aff_stat->setScene(scene);

    // Redimensionner la scène pour s'adapter au QGraphicsView
    chartView->resize(ui->aff_stat->size());  // Ajuste la taille du chartView à celle de stat_e
}



void MainWindow::on_actualiser_clicked()
{
        Employe e; // Créez une instance de la classe Employe
        e.afficherRappelPoints(40); // Appelez la fonction avec un seuil de 40 points
    }


void MainWindow::on_menu_employe_clicked()
{
    ui->gestion_employe->show();
    ui->gestion_client->hide();
    ui->menu->hide();
    ui->login->hide();

}

void MainWindow::on_bt_menu_clicked()
{
    ui->gestion_employe->hide();
    ui->gestion_client->hide();
    ui->menu->show();
    ui->login->hide();

}

void MainWindow::on_menu_client_clicked()
{
    ui->gestion_employe->hide();
    ui->gestion_client->show();
    ui->menu->hide();
    ui->login->hide();

}

void MainWindow::on_bt_menu_2_clicked()
{
    ui->gestion_employe->hide();
    ui->gestion_client->hide();
    ui->menu->show();
    ui->login->hide();

}

void MainWindow::on_ajouter_2_clicked()
{
    QString nom = ui->nom_2->text();
    QString prenom = ui->prenom_2->text();
    QString role = ui->role_2->currentText();
    double salaire = ui->salaire_2->text().toDouble();
    QString email=ui->email->text();

    if (nom.isEmpty() || prenom.isEmpty() || role.isEmpty() || salaire <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs correctement !");
        return;
    }

    Employe e(0, nom, prenom, role, salaire,email);
    if (e.ajouter()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        ui->aff->setModel(e.afficher());
        afficherStatistiques();

    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout de l'employé !");
    }}

void MainWindow::on_annuler_2_clicked()
{
    ui->nom_2->clear();
    ui->prenom_2->clear();
    ui->salaire_2->clear();
    ui->role_2->setCurrentIndex(0);
    ui->id_m->clear();
    ui->id_supp->clear();

    QMessageBox::information(this, "Réinitialisé", "Tous les champs ont été vidés !");
}

void MainWindow::on_ajouter_f_2_clicked()
{
    int id_employe = ui->id_em_2->text().toInt();
    QString feedback = ui->feedback_2->currentText(); // Assurez-vous que la ComboBox contient '+' et '-'

    if (id_employe <= 0 || (feedback != "+" && feedback != "-")) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir correctement les champs !");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO feedback (id_f, id_employe, feedback) "
                  "VALUES (seq_feedback.NEXTVAL, :id_employe, :feedback)");
    query.bindValue(":id_employe", id_employe);
    query.bindValue(":feedback", feedback);

    if (query.exec()) {
        // Mise à jour des points dans `emp_f`
        int points_change = (feedback == "+") ? 10 : -10;

        QSqlQuery updateQuery;
        updateQuery.prepare("MERGE INTO emp_f USING DUAL "
                            "ON (id_employe = :id_employe) "
                            "WHEN MATCHED THEN "
                            "    UPDATE SET points = points + :points "
                            "WHEN NOT MATCHED THEN "
                            "    INSERT (id_employe, feedback, points) VALUES (:id_employe, :feedback, :points)");
        updateQuery.bindValue(":id_employe", id_employe);
        updateQuery.bindValue(":feedback", feedback);
        updateQuery.bindValue(":points", points_change);

        if (updateQuery.exec()) {
            QMessageBox::information(this, "Succès", "Feedback ajouté avec succès !");
            ui->aff_feed_2->setModel(e.afficherEmpFeedback());

        } else {
            QMessageBox::warning(this, "Erreur", "Échec de la mise à jour des points !");
        }
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'ajout du feedback !");
    }
}



void MainWindow::on_pushButton_clicked()
{
    int cin = ui->lineEdit->text().toInt();
       QString nom = ui->lineEdit_2->text();
       QString prenom = ui->lineEdit_3->text();
        int phone = ui->lineEdit_4->text().toInt();
       QString pays = ui->lineEdit_5->text();
       QString payement = ui->lineEdit_6->text();
        QString dureeArrivee = ui->lineEdit_8->text();
       Client C(cin,nom,prenom,phone,pays,payement,dureeArrivee);

       bool test = C.ajouter();
       C.ajouter_historique(cin,nom,prenom,pays,"ajout");

       if (test)
       {
           ui->tableView->setModel(clt.afficher());
              ui->histo->setModel(clt.afficher_historique0());
               ui->lineEdit->clear();
               ui->lineEdit_2->clear();
               ui->lineEdit_3->clear();
               ui->lineEdit_4->clear();
               ui->lineEdit_5->clear();
               ui->lineEdit_6->clear();
                ui->lineEdit_8->clear();
        }
       else
           QMessageBox :: critical(nullptr,QObject::tr("NOT OK"),
                                   QObject::tr("ajout Non effectuee\n"
                                               "Click cancel o exit."), QMessageBox::Cancel);
}

void MainWindow::on_pushButton_2_clicked()
{
    int cin = ui->lineEdit->text().toInt();
       QString nom = ui->lineEdit_2->text();
       QString prenom = ui->lineEdit_3->text();
        int phone = ui->lineEdit_4->text().toInt();
       QString pays = ui->lineEdit_5->text();
       QString payement = ui->lineEdit_6->text();
        QString dureeArrivee = ui->lineEdit_8->text();
       Client C(cin,nom,prenom,phone,pays,payement,dureeArrivee);

      bool test=C.modifier(cin);
       C.ajouter_historique(cin,nom,prenom,pays,"modification");
        ui->histo->setModel(clt.afficher_historique0());
       if(test)
       {
           QMessageBox::information(nullptr, QObject::tr("Suppression réussie"),
                                    QObject::tr("Laffectation a été effectuée avec succès.\n"
                                                "Cliquez sur OK pour continuer."),
                                    QMessageBox::Ok);
            }
       else
           QMessageBox::critical(nullptr, QObject::tr("NOT OK"),
                       QObject::tr("failed.\n"
                                   "Click Cancel to exit."), QMessageBox::Cancel);
       ui->lineEdit->clear();
       ui->lineEdit_2->clear();
       ui->lineEdit_3->clear();
       ui->lineEdit_4->clear();
       ui->lineEdit_5->clear();
       ui->lineEdit_6->clear();
        ui->lineEdit_8->clear();
  ui->tableView->setModel(clt.afficher());
}

void MainWindow::on_rechercher_2_textChanged(const QString &cher)
{
                ui->tableView->setModel(clt.chercher(cher));
}

void MainWindow::on_pushButton_3_clicked()
{
    clt.setCin(ui->lineEdit_7->text().toInt()); // Set CIN from input
    bool test = clt.supprimer(clt.getCin());    // Attempt to delete client
   // Client c = clt.getClientByCin(ui->lineEdit_7->text().toInt());
  clt.ajouter_historique(ui->lineEdit_7->text().toInt(), ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->lineEdit_5->text(),"suppression");
    if (test) // If deletion is successful
    {
        ui->tableView->setModel(clt.afficher()); // Refresh the table view
          ui->histo->setModel(clt.afficher_historique0());
        QMessageBox::information(nullptr, QObject::tr("Suppression réussie"),
                                 QObject::tr("La suppression a été effectuée avec succès.\n"
                                             "Cliquez sur OK pour continuer."),
                                 QMessageBox::Ok);
        ui->lineEdit_7->clear(); // Clear the input field
    }
    else // If deletion fails
    {
        QMessageBox::critical(nullptr, QObject::tr("Suppression échouée"),
                              QObject::tr("La suppression n'a pas pu être effectuée.\n"
                                          "Cliquez sur Annuler pour quitter."),
                              QMessageBox::Cancel);
    }
}


void MainWindow::on_radioButton_clicked()
{
    ui->tableView->setModel(clt.show_Desc_name());

}

void MainWindow::on_radioButton_2_clicked()
{
    ui->tableView->setModel(clt.show_Asc_name());

}

void MainWindow::on_his_but_clicked()
{
    Client client;
     QString type=ui->typehistorique->currentText();
     ui->histo->setModel(client.afficher_historique(type));
}

void MainWindow::on_stat_clicked()
{
    s = new stat_combo();
      s->setWindowTitle("statistique ComboBox");
      s->choix_pie();
      s->show();
}

void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    QAbstractItemModel* model = ui->tableView->model();

           int row = index.row();



           QString CIN = model->data(model->index(row, 0)).toString();
           QString NOM = model->data(model->index(row, 1)).toString();
           QString PRENOM = model->data(model->index(row, 2)).toString();
           QString PHONE = model->data(model->index(row, 3)).toString();
           QString PAYS = model->data(model->index(row, 4)).toString();
           QString PAIEMENT = model->data(model->index(row, 5)).toString();
           QString duree = model->data(model->index(row, 6)).toString();


   ui->lineEdit->setText(CIN);
   ui->lineEdit_2->setText(NOM);
   ui->lineEdit_3->setText(PRENOM);
   ui->lineEdit_4->setText(PHONE);
   ui->lineEdit_5->setText(PAYS);
   ui->lineEdit_6->setText(PAIEMENT);
   ui->lineEdit_8->setText(duree);
   // Using QrCodegen to create a QR code from Facture attributes
                    QString text = "CIN: " + CIN + "\n"
                                    "NOM: " + NOM + "\n"
                                 "PRENOM: " + PRENOM + "\n"
                                 "PHONE: " + PHONE + "\n"
                                 "PAYS: " + PAYS + "\n"
                                    "PAIEMENT: " + PAIEMENT + "\n"
                        "duree: " + duree + "\n";

    using namespace qrcodegen;
   QrCode qr = QrCode::encodeText(text.toUtf8().data(), QrCode::Ecc::MEDIUM);

                    qint32 sz = qr.getSize();
                    QImage im(sz, sz, QImage::Format_RGB32);
                    QRgb black = qRgb(0, 0, 0);
                    QRgb white = qRgb(255, 255, 255);

                    for (int y = 0; y < sz; y++) {
                        for (int x = 0; x < sz; x++) {
                            im.setPixel(x, y, qr.getModule(x, y) ? black : white);
                        }
                    }
                    ui->qrcode->setPixmap(QPixmap::fromImage(im.scaled(200, 200, Qt::KeepAspectRatio, Qt::FastTransformation), Qt::MonoOnly));
}

void MainWindow::on_pushButton_4_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

    const int rowCount = ui->tableView->model()->rowCount();
    const int columnCount = ui->tableView->model()->columnCount();

    out << "<html>\n"
           "<head>\n"
           "<meta Content=\"Text/html; charset=Windows-1251\">\n"
           "<title>%1</title>\n"
           "<style>\n"
           "table {\n"
           "    width: 100%;\n"
           "    border-collapse: collapse;\n"
           "}\n"
           "th, td {\n"
           "    padding: 8px;\n"
           "    text-align: left;\n"
           "    border-bottom: 1px solid #ddd;\n"
           "}\n"
           "tr:nth-child(even) {\n"
           "    background-color: #f2f2f2;\n"
           "}\n"
           "</style>\n"
           "</head>\n"
           "<body bgcolor=#ffffff link=#5000A0>\n"
           "<center> <H1>Liste des formations</H1></center><br/><br/>\n"
           "<img src=\"path/to/your/image.jpg\" alt=\"Description of image\" style=\"max-width: 100%; height: auto;\">\n"
           "<table>\n";

    // headers
    out << "<thead><tr bgcolor=#f0f0f0> <th>Numero</th>";
    for (int column = 0; column < columnCount; column++)
    {
        if (!ui->tableView->isColumnHidden(column))
        {
            out << QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
        }
    }
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++)
    {
        out << "<tr> <td>" << row + 1 << "</td>";
        for (int column = 0; column < columnCount; column++)
        {
            if (!ui->tableView->isColumnHidden(column))
            {
                QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, column)).toString().simplified();
                out << QString("<td>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }



    QString fileName = QFileDialog::getSaveFileName((QWidget *)0, "Sauvegarder en PDF", QString(), "*.pdf");
    if (QFileInfo(fileName).suffix().isEmpty())
    {
        fileName.append(".pdf");
    }

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(strStream);
    doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    doc.print(&printer);

}

void MainWindow::on_login_2_clicked()
{
    QString email = ui->email_2->text().toLower().trimmed();
    QString password = ui->password->text().toLower().trimmed();

    if (!email.contains("@smarthotel")) {
        QMessageBox::warning(this, "Authentication", "Invalid email format.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYE WHERE email = :email AND password = :password");
    query.bindValue(":email", email);
    query.bindValue(":password", password);

    if (query.exec()) {
        if (query.next()) {
            QMessageBox::information(this, "Authentication", "Login successful!");
            ui->login->hide();
            ui->gestion_employe->hide();
            ui->gestion_client->hide();
            ui->menu->show();
        } else {
            QMessageBox::warning(this, "Authentication", "Invalid email or password.");
        }
    } else {
        QMessageBox::critical(this, "Error", "Query error: " + query.lastError().text());
    }
}

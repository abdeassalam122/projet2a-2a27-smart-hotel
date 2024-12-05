#include "employe.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QPdfWriter>
#include <QPainter>
#include <QPrinter>
#include <QFile>
#include <QTextDocument>
#include <QDate>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
// Constructeur
Employe::Employe(int id, QString nom, QString prenom, QString role, double salaire,QString email)
    : id(id), nom(nom), prenom(prenom), role(role), salaire(salaire),email(email) {}

// Getters
int Employe::getId() const { return id; }
QString Employe::getNom() const { return nom; }
QString Employe::getPrenom() const { return prenom; }
QString Employe::getRole() const { return role; }
double Employe::getSalaire() const { return salaire; }

// Setters
void Employe::setId(int id) { this->id = id; }
void Employe::setNom(QString nom) { this->nom = nom; }
void Employe::setPrenom(QString prenom) { this->prenom = prenom; }
void Employe::setRole(QString role) { this->role = role; }
void Employe::setSalaire(double salaire) { this->salaire = salaire; }

// Ajouter un employé
bool Employe::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO employe (id_e, nom, prenom, role, salaire,email) "
                  "VALUES (seq_employe.NEXTVAL, :nom, :prenom, :role, :salaire,:email)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":role", role);
    query.bindValue(":salaire", salaire);

    return query.exec();
}

// Modifier un employé
bool Employe::modifier() {
    QSqlQuery query;
    query.prepare("UPDATE employe SET nom = :nom, prenom = :prenom, role = :role, salaire = :salaire ,email=:email"
                  "WHERE id_e = :id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":role", role);
    query.bindValue(":salaire", salaire);
        query.bindValue(":email", email);
    return query.exec();
}

// Supprimer un employé
bool Employe::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM employe WHERE id_e = :id");
    query.bindValue(":id", id);
    return query.exec();
}

// Afficher les employés
QSqlQueryModel* Employe::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employe");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Role"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Email"));

    return model;
}

// Afficher le tableau `emp_f`
QSqlQueryModel* Employe::afficherEmpFeedback() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT emp.id_e, emp.nom, emp.prenom, emp_f.feedback, emp_f.points "
                    "FROM employe emp "
                    "LEFT JOIN emp_f ON emp.id_e = emp_f.id_employe");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id Employe"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Feedback"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Points"));
    return model;
}

// Vérifier l'ID d'un employé
bool Employe::verifierId(int id) {
    QSqlQuery query;
    query.prepare("SELECT id_e FROM employe WHERE id_e = :id");
    query.bindValue(":id", id);
    query.exec();
    return query.next(); // Retourne vrai si un résultat est trouvé
}

// Recherche par critère
QSqlQueryModel* Employe::rechercher(QString critere, QString valeur) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = QString("SELECT * FROM employe WHERE %1 LIKE '%%2%'").arg(critere, valeur);
    model->setQuery(queryStr);
    return model;
}
// Trier par critère
QSqlQueryModel* Employe::trier(QString critere) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = QString("SELECT * FROM employe ORDER BY %1").arg(critere);
    model->setQuery(queryStr);
    return model;
}
// Générer le modèle HTML pour le PDF
QString Employe::genererContenuPDF() {
    // Définir un modèle HTML avec du CSS pour la mise en page
    QString html = R"(
    <html>
    <head>
        <style>
            body {
                font-family: Arial, sans-serif;
                margin: 0;
                padding: 0;
            }
            h1 {
                text-align: center;
                color: #333;
                margin-bottom: 20px;
            }
            table {
                width: 100%;
                border-collapse: collapse;
                margin: 0;
            }
            th, td {
                border: 1px solid #dddddd;
                padding: 10px;
                text-align: left;
                font-size: 12px;
            }
            th {
                background-color: #f2f2f2;
                font-weight: bold;
            }
            .footer {
                text-align: center;
                font-size: 12px;
                color: #666;
                margin-top: 30px;
            }
        </style>
    </head>
    <body>
        <h1>Rapport des Employes</h1>
        <table>
            <tr>
                <th>ID employe</th>
                <th>Nom</th>
                <th>Prenom Client</th>
                <th>Role</th>
                <th>Salaire (DT)</th>
            </tr>
    )";

    // Ajouter les employe à la table dans la boucle
    QSqlQuery query("SELECT * FROM employe");
    while (query.next()) {
        int id_e = query.value("id_e").toInt();
        QString nom = query.value("nom").toString();
        QString prenom = query.value("prenom").toString();

        QString role = query.value("role").toString();
        int salaire = query.value("salaire").toInt();

        html += QString(
                    "<tr>"
                    "<td>%1</td>"
                    "<td>%2</td>"
                    "<td>%3</td>"
                    "<td>%4</td>"
                    "<td>%5</td>"
                    "</tr>"
                    )
                    .arg(id_e)
                    .arg(nom)
                    .arg(prenom)
                    .arg(role)
                    .arg(salaire);
    }

    html += QString(R"(
        </table>
        <div class='footer'>
            <p>Ce rapport a été généré automatiquement le %1.</p>
        </div>
    </body>
    </html>
    )").arg(QDate::currentDate().toString("dd/MM/yyyy"));

    return html;
}

// Gérer la génération du PDF
void Employe::genererPDF(const QString& fichierPDF) {
    QString contenuHTML = genererContenuPDF(); // Récupérer le contenu HTML généré

    QTextDocument document;
    document.setHtml(contenuHTML);

    // Configurer le QPrinter pour créer un PDF
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fichierPDF);

    // Définir les marges de la page
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    // Imprimer le document dans le fichier PDF
    document.print(&printer);
}

QPieSeries* Employe::statistiques() {
    QPieSeries* series = new QPieSeries();

    // Requête SQL pour compter les employes par type_paiement
    QSqlQuery query;
    query.prepare("SELECT role, COUNT(*) FROM employe GROUP BY role");

    if (query.exec()) {
        while (query.next()) {
            QString role = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(role, count);
        }
    } else {
        qDebug() << "Erreur lors de la récupération des statistiques par role : " << query.lastError().text();
    }

    return series;
}



QStringList verifierPoints(int seuil) {
    QStringList employes;
    QSqlQuery query;
    query.prepare("SELECT id_employe FROM emp_f WHERE points >= :seuil");
    query.bindValue(":seuil", seuil);

    if (query.exec()) {
        while (query.next()) {
            int id_employe = query.value(0).toInt();
            employes.append(QString::number(id_employe));
        }
    } else {
        qDebug() << "Erreur lors de la vérification des points :" << query.lastError();
    }

    return employes;
}

void Employe::afficherRappelPoints(int seuil) {
    QSqlQuery query;
    query.prepare("SELECT e.id_e, e.nom, e.prenom, ef.points "
                  "FROM employe e "
                  "JOIN emp_f ef ON e.id_e = ef.id_employe "
                  "WHERE ef.points >= :seuil");
    query.bindValue(":seuil", seuil);

    if (query.exec()) {
        while (query.next()) {
            int id = query.value("id_e").toInt();
            QString nom = query.value("nom").toString();
            QString prenom = query.value("prenom").toString();
            int points = query.value("points").toInt();

            QMessageBox::information(nullptr, "Rappel Congé",
                                     QString("C'est le temps de congé pour l'employé %1 %2 (ID: %3) avec %4 points.")
                                         .arg(nom).arg(prenom).arg(id).arg(points));
        }
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de récupérer les données de feedback.");
    }
}


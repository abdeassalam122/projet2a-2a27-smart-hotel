#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QPieSeries>
using namespace  QtCharts;
class Employe {
private:
    int id;
    QString nom;
    QString prenom;
    QString role;
    double salaire;
QString email;
public:
    // Constructeurs
    Employe() {}
    Employe(int id, QString nom, QString prenom, QString role, double salaire,QString email);

    // Getters
    int getId() const;
    QString getNom() const;
    QString getPrenom() const;
    QString getRole() const;
    double getSalaire() const;
    QString getemail() const;

    // Setters
    void setId(int id);
    void setNom(QString nom);
    void setPrenom(QString prenom);
    void setRole(QString role);
    void setSalaire(double salaire);

    // Méthodes CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* afficherEmpFeedback();
    bool verifierId(int id);


    // metiers simple
    QSqlQueryModel* rechercher(QString critere, QString valeur);
    QSqlQueryModel* trier(QString critere);
    QString genererContenuPDF();

    void genererPDF(const QString& fichierPDF);
    QPieSeries* statistiques();

    // Fonction pour récupérer la liste des employés ayant des points >= seuil
    QStringList verifierPoints(int seuil);

    // Fonction pour afficher les employés ayant atteint un seuil de points
    void afficherRappelPoints(int seuil); // Déclarez la fonction


};

#endif // EMPLOYE_H

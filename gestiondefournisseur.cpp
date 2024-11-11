#include "gestiondefournisseur.h"
#include "ui_gestiondefournisseur.h"
#include "fournisseur.h"
#include <QMessageBox>

gestiondefournisseur::gestiondefournisseur(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gestiondefournisseur)
{
    ui->setupUi(this);
    ui->tableView->setModel(Ftmp.afficher());
}

gestiondefournisseur::~gestiondefournisseur()
{
    delete ui;
}

void gestiondefournisseur::on_pushButton_ajouter_clicked()
{
    int id = ui->lineEdit_ID->text().toInt();
    QString Nom_F = ui->lineEdit_nom->text();
    QString Adresse = ui->lineEdit_adresse->text();
    QString Email = ui->lineEdit_email->text();
    QString Telephone = ui->lineEdit_telephone->text();
    QString Secteur = ui->lineEdit_secteur->text();

    Fournisseur F(id, Nom_F, Adresse, Email, Telephone, Secteur);

    bool test = F.ajouter();

    if (test)
    {
        ui->tableView->setModel(Ftmp.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Ajout effectué\n"), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Ajout non effectué.\n"), QMessageBox::Cancel);
    }
}

void gestiondefournisseur::on_pushButton_supprimer_clicked()
{
    int id = ui->lineEdit_IDS->text().toInt();
    bool test = Ftmp.supprimer(id);

    if (test)
    {
        ui->tableView->setModel(Ftmp.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"), QObject::tr("Suppression effectuée\n"), QMessageBox::Cancel);
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"), QObject::tr("Suppression non effectuée.\n"), QMessageBox::Cancel);
    }
}
void gestiondefournisseur::on_pushButton_modifier_clicked()
{
    int id = ui->lineEdit_id->text().toInt();
    QString Nom_F = ui->lineEdit_nom->text();
    QString Adresse = ui->lineEdit_adresse->text();
    QString Email = ui->lineEdit_email->text();
    QString Telephone = ui->lineEdit_telephone->text();
    QString Secteur = ui->lineEdit_secteur->text();

    Fournisseur F(id, Nom_F, Adresse, Email, Telephone, Secteur);

    if (F.modifier())
    {
        QMessageBox::information(nullptr, QObject::tr("OK"),
                    QObject::tr("Modification effectuée\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
        ui->tableView->setModel(Ftmp.afficher());  // Rafraîchir la vue de table
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                    QObject::tr("Modification non effectuée.\n"
                                "Click Cancel to exit."), QMessageBox::Cancel);
    }
}


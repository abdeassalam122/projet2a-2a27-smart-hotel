#ifndef GESTIONDEFOURNISSEUR_H
#define GESTIONDEFOURNISSEUR_H

#include <QMainWindow>
#include "fournisseur.h"  // Make sure the fournisseur.h file is included
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include "arduino.h"
#include "connection.h"


QT_BEGIN_NAMESPACE
namespace Ui { class gestiondefournisseur; }
QT_END_NAMESPACE

class gestiondefournisseur : public QMainWindow
{
    Q_OBJECT

public:
    gestiondefournisseur(QWidget *parent = nullptr);
    ~gestiondefournisseur();
    void envoyerEmailConfirmation(const QString &destinataire, const QString &sujet, const QString &message);
    void showSectorPieChart();

private:
    Ui::gestiondefournisseur *ui;
    Fournisseur Ftmp;
    bool ordreAscendant;
    Arduino *arduino;  // Arduino object to control buzzer and communication
       QSqlDatabase db;





private slots:  // Use private slots for slot methods
    void on_pushButton_ajouter_2_clicked();
    void on_pushButton_supprimer_2_clicked();
    void on_pushButton_modifier_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_7_clicked();


};

#endif // GESTIONDEFOURNISSEUR_H

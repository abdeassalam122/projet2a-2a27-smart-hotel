#ifndef GESTIONDEFOURNISSEUR_H
#define GESTIONDEFOURNISSEUR_H

#include <QMainWindow>
#include "fournisseur.h"  // Make sure the fournisseur.h file is included

QT_BEGIN_NAMESPACE
namespace Ui { class gestiondefournisseur; }
QT_END_NAMESPACE

class gestiondefournisseur : public QMainWindow
{
    Q_OBJECT

public:
    gestiondefournisseur(QWidget *parent = nullptr);
    ~gestiondefournisseur();

private:
    Ui::gestiondefournisseur *ui;
    Fournisseur Ftmp;

private slots:  // Use private slots for slot methods
    void on_pushButton_ajouter_clicked();   // Slot for adding a supplier
    void on_pushButton_supprimer_clicked(); // Slot for deleting a supplier
    void on_pushButton_modifier_clicked();
};

#endif // GESTIONDEFOURNISSEUR_H

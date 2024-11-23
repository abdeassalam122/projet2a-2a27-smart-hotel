#ifndef GESTION__RESERVATION_H
#define GESTION__RESERVATION_H

#include <QMainWindow>
#include "reservation.h"  // Include the Reservation class
#include <QSqlTableModel> // Include this if using a model with database
#include <QStandardItemModel> // For using QStandardItemModel
#include <QTableView>

QT_BEGIN_NAMESPACE
namespace Ui { class gestion__reservation; }
QT_END_NAMESPACE

class gestion__reservation : public QMainWindow
{
    Q_OBJECT

public:
    explicit gestion__reservation(QWidget *parent = nullptr); // Le parent est de type QWidget (ou QMainWindow)
    ~gestion__reservation();
    // gestion__reservation.h
    void updateHistorique(const QString &action, const QString &details);


private slots:
    void on_pushButton_clicked();    // Slot for adding a reservation
    void on_pushButton_2_clicked();  // Slot for modifying a reservation
    void on_pushButton_3_clicked();  // Slot for deleting a reservation
    void on_pushButton_T_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_Stats_clicked();
    void on_pushButton_5_clicked();
    // Declare the showHistorique function as a slot
    void showHistorique();
    void on_pushButton_6_clicked();
private:
    Ui::gestion__reservation *ui;  // Pointer to the UI
    Reservation Rtmp;              // Instance of Reservation
    QSqlTableModel *historiqueModel;
    // Déclarez tableView_2 si ce n'est pas déjà fait
       QTableView *tableView_2;  // Ajoutez cette ligne si tableView_2 n'est pas déjà déclaré
};

#endif // GESTION__RESERVATION_H

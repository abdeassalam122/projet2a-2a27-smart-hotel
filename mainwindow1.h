#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "arduino.h"
#include "client.h"
#include "stat_combo.h"
#include "arduino.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_modifier_clicked();

    void on_recuperer_clicked();

    void on_supprimer_clicked();



    void on_trie_clicked();

    void on_rechercher_clicked();

    void on_pdf_clicked();

    void   afficherStatistiques();


    void on_actualiser_clicked();

    void on_menu_employe_clicked();

    void on_bt_menu_clicked();

    void on_menu_client_clicked();

    void on_bt_menu_2_clicked();

    void on_ajouter_2_clicked();

    void on_annuler_2_clicked();

    void on_ajouter_f_2_clicked();


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_rechercher_2_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();


    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_his_but_clicked();

    void on_stat_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_4_clicked();
    void readFromArduino();

    void on_login_2_clicked();

private:
    Ui::MainWindow *ui;
    Client clt;
     stat_combo * s;
     arduino A;

};
#endif // MAINWINDOW_H

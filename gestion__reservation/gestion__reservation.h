#ifndef GESTION__RESERVATION_H
#define GESTION__RESERVATION_H

#include <QMainWindow>
#include "reservation.h"  // Include the Reservation class
#include "dialog.h"

QT_BEGIN_NAMESPACE

namespace Ui { class gestion__reservation; }
QT_END_NAMESPACE

class gestion__reservation : public QMainWindow
{
    Q_OBJECT

public:
    explicit gestion__reservation(QWidget *parent = nullptr);
    ~gestion__reservation();

private slots:
    void on_pushButton_clicked();    // Slot for adding a reservation
    void on_pushButton_2_clicked();  // Slot for modifying a reservation
    void on_pushButton_3_clicked();  // Slot for deleting a reservation
    void on_pushButton_T_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_Stats_clicked();
    void on_pushButtonSend_clicked();  // Slot to open Dialog

private:
    Ui::gestion__reservation *ui;  // Pointer to the UI
    Reservation Rtmp;              // Instance of Reservation
    Dialog *dialogInstance = nullptr;  // Instance of Dialog (unique)
};

#endif // GESTION__RESERVATION_H

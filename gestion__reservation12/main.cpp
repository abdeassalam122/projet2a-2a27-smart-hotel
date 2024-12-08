#include "gestion__reservation.h"  // Updated to include the correct header file
#include <QApplication>
#include <QMessageBox>
#include "connection.h"  // Database connection class

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createConnection();

    gestion__reservation w;  // Updated to use gestion__reservation instead of MainWindow
    if (test) {
        w.show();
        QMessageBox::information(nullptr, QObject::tr("Database Connection"),
                                 QObject::tr("Connection successful.\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }

    return a.exec();
}

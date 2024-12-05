#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include "connection.h"
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    connection c;
    bool test = c.createconnection();

    if (!test) {
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\n Click Cancel to exit."),
                              QMessageBox::Cancel);
        return -1;
    }

   /* Login login;
    if (login.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }*/
    MainWindow w;
    w.show();
    return a.exec();
    return 0; // Quitter l'application si login échoue
}

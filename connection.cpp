#include "connection.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Connection::Connection() {
    // Constructeur vide
}

bool Connection::createConnection()
{bool test=false;
QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
db.setDatabaseName("CPP_PROJECT");//inserer le nom de la source de données
db.setUserName("maram");//inserer nom de l'utilisateur
db.setPassword("azerty");//inserer mot de passe de cet utilisateur

if (db.open())
test=true;


    return  test;
}


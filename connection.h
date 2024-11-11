
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class Connection
{
    QSqlDatabase db;

public:
    Connection();  // Constructeur
    bool createConnection();  // Méthode pour créer la connexion
    void closeConnection();  // Méthode pour fermer la connexion
};

#endif // CONNECTION_H


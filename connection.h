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
    Connection();
    bool createConnection();    // Create database connection
    void closeConnection();     // Close database connection

};

#endif // CONNECTION_H

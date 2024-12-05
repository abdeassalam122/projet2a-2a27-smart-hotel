#include "connection.h"

connection::connection(){}

bool connection::createconnection(){
    db = QSqlDatabase::addDatabase("QODBC");
    bool test=false;
    db.setDatabaseName("CPP_PROJECT");//CPP_PROJECT
    db.setUserName("abdeassalam"); //Amine
    db.setPassword("oracle"); //oracle
    if (db.open()) test=true;
    return test;
}

void connection::closeconnection(){db.close();}

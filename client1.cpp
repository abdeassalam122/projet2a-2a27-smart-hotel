#include "client.h"
#include <QDebug>
#include <QDateTime>
Client::Client()
{

}
Client::Client(int cin,  QString &nom,  QString &prenom,  int &phone,
               QString &pays,  QString &payement,  QString &dureeArrivee){
    this->cin = cin;
     this->nom = nom;
     this->prenom = prenom;
     this->phone = phone;
     this->pays = pays;
     this->payement = payement;
     this->dureeArrivee = dureeArrivee;
}
bool Client :: ajouter(){
    QSqlQuery query;
    QString res =  QString :: number(cin);
    query.prepare("insert into client (cin,nom,prenom,phone,pays,payement,dureeArrivee)" "values (:cin,:nom,:prenom,:phone,:pays,:payement,:dureeArrivee)");
    query.bindValue(":cin",res);
    query.bindValue(":nom",nom);
    query.bindValue(":prenom",prenom);
    query.bindValue(":phone",phone);
     query.bindValue(":pays",pays);
     query.bindValue(":payement",payement);
      query.bindValue(":dureeArrivee",dureeArrivee);
     return query.exec();
}

bool Client :: supprimer(int cin){
    QSqlQuery query;
           QString res = QString::number(cin);
           query.prepare("Delete from client where cin= :cin");
        query.bindValue(":cin",res);

           return query.exec();
}
QSqlQueryModel * Client :: afficher(){
    QSqlQueryModel * model = new QSqlQueryModel();
    model->setQuery("select * from client");
    model->setHeaderData(0,Qt::Horizontal,QObject::tr("CIN"));
    model->setHeaderData(1,Qt::Horizontal,QObject::tr("NOM"));
    model->setHeaderData(2,Qt::Horizontal,QObject::tr("Prenom"));
    model->setHeaderData(3,Qt::Horizontal,QObject::tr("phone"));
    model->setHeaderData(4,Qt::Horizontal,QObject::tr("pays"));
      model->setHeaderData(5,Qt::Horizontal,QObject::tr("payement"));
    model->setHeaderData(5,Qt::Horizontal,QObject::tr("dureeArrivee"));

    return model;
}
bool Client :: modifier(int cin)
{
    QSqlQuery query;
        query.prepare("update client set cin=:cin,nom=:nom,prenom=:prenom,phone=:phone,pays=:pays,payement=:payement,dureeArrivee=:dureeArrivee where cin= :cin");


        query.bindValue(":cin",cin);
        query.bindValue(":nom",nom);
        query.bindValue(":prenom",prenom);
        query.bindValue(":phone",phone);
         query.bindValue(":pays",pays);
         query.bindValue(":payement",payement);
           query.bindValue(":dureeArrivee",dureeArrivee);

        return query.exec();

}
QSqlQueryModel * Client::chercher(QString cher)
    {
        QSqlQueryModel * model= new QSqlQueryModel();

     model->setQuery("select * from client where upper(cin) LIKE upper('%"+cher+"%')");
    return model;
    }
QSqlQueryModel * Client::show_Desc_name()
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from client ORDER BY cin DESC ");
     model->setHeaderData(0,Qt::Horizontal,QObject::tr("CIN"));
     model->setHeaderData(1,Qt::Horizontal,QObject::tr("NOM"));
     model->setHeaderData(2,Qt::Horizontal,QObject::tr("Prenom"));
     model->setHeaderData(3,Qt::Horizontal,QObject::tr("phone"));
     model->setHeaderData(4,Qt::Horizontal,QObject::tr("pays"));
       model->setHeaderData(5,Qt::Horizontal,QObject::tr("payement"));
     model->setHeaderData(5,Qt::Horizontal,QObject::tr("dureeArrivee"));
     return model;
}
QSqlQueryModel * Client::show_Asc_name()
{
    QSqlQueryModel* model = new QSqlQueryModel();
     model->setQuery("select * from client ORDER BY cin ASC ");
     model->setHeaderData(0,Qt::Horizontal,QObject::tr("CIN"));
     model->setHeaderData(1,Qt::Horizontal,QObject::tr("NOM"));
     model->setHeaderData(2,Qt::Horizontal,QObject::tr("Prenom"));
     model->setHeaderData(3,Qt::Horizontal,QObject::tr("phone"));
     model->setHeaderData(4,Qt::Horizontal,QObject::tr("pays"));
       model->setHeaderData(5,Qt::Horizontal,QObject::tr("payement"));
     model->setHeaderData(5,Qt::Horizontal,QObject::tr("dureeArrivee"));
     return model;
}
bool Client::ajouter_historique(int cin,QString nom,QString prenom,QString pays,QString type)
 {
     QSqlQuery query;
    QString res =  QString :: number(cin);


     query.prepare("INSERT INTO history (cin,nom,prenom,pays,date_historique,type) "
                   "VALUES (:cin, :nom, :prenom,:pays,:date_historique,:type)");
     query.bindValue(":cin",res);
     query.bindValue(":nom", nom);
     query.bindValue(":prenom", prenom);
     query.bindValue(":pays", pays);
     query.bindValue(":date_historique", QDateTime::currentDateTime());
      query.bindValue(":type", type);
     return    query.exec();
 }
QSqlQueryModel * Client::afficher_historique0()
{
    QSqlQueryModel * model= new QSqlQueryModel();


        model->setQuery("select * FROM history");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Cin "));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("date d'historique"));


    return model;

    }

QSqlQueryModel * Client::afficher_historique(QString type)
{
    QSqlQueryModel * model= new QSqlQueryModel();
     QString a;
     if(type=="tout")
     {        model->setQuery("select * FROM history");
         model->setHeaderData(0, Qt::Horizontal, QObject::tr("Cin "));
         model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
         model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
         model->setHeaderData(3, Qt::Horizontal, QObject::tr("date d'historique"));
     }
     else{
        model->setQuery("select * FROM history  WHERE type LIKE '%"+type+"%'  ");
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("Cin "));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("date d'historique"));
}
    return model;
}
bool Client::supprimerh(int cin)
{

    QSqlQuery query;

         query.prepare("DELETE from history where cin=:cin ");
         query.bindValue(":cin", cin);
     return     query.exec();

}





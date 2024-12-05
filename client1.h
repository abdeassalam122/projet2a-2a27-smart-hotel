#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Client
{
public:
    Client();
    Client(int cin,  QString &nom,  QString &prenom,  int &phone,
               QString &pays,  QString &payement,  QString &dureeArrivee);

    int getCin(){
        return cin;}
    QString getNom(){
        return nom;
    }
    QString getPrenom() {
        return prenom;
    }
    int getPhone() {
        return phone;
    }
    QString getPays() {
        return pays;
    }
    QString getPayement() {
        return payement;
    }
    QString getDureeArrivee() {
        return dureeArrivee;
    }


       void setCin(int cin){
          this->cin = cin;
       }
       void setNom( QString nom)
       {
           this->nom = nom;
       }
       void setPrenom( QString prenom){
           this->prenom = prenom;
       }
       void setPhone( int phone){
           this->phone = phone;
       }
       void setPays( QString pays){
           this->pays = pays;
       }
       void setPayement( QString payement){
            this->payement = payement;
       }
       void setDureeArrivee( QString dureeArrivee){
           this->dureeArrivee = dureeArrivee;
       }

       bool ajouter();
       bool supprimer(int cin);
       QSqlQueryModel * afficher();
        bool modifier(int cin);
          QSqlQueryModel * chercher(QString cher);
          QSqlQueryModel * show_Desc_name();
          QSqlQueryModel * show_Asc_name();

          //histoqiue
         QSqlQueryModel * afficher_historique0();
          QSqlQueryModel * afficher_historique(QString type);
        bool ajouter_historique(int cin,QString nom,QString prenom,QString pays,QString type);
         bool supprimerh(int id);
         Client getClientByCin(int cin);

   private:
       int cin;
       QString nom;
       QString prenom;
       int phone;
       QString pays;
       QString payement;
       QString dureeArrivee;
};

#endif // CLIENT_H

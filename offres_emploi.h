#ifndef OFFRES_EMPLOI_H
#define OFFRES_EMPLOI_H
#include <QString>
#include <QDate>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

class offres_emploi
{
public:
    offres_emploi(){}
    offres_emploi(QString,QString,QDate,QDate,int);
private:
    int id_offre, id_entreprise;
    QString titre, description;
    QDate date_publication, date_expiration;
public:
    //setters
    void setID(int n);
    void setIdEntreprise(int n);
    void setTitre(QString n);
    void setDescription(QString n);
    void setDatePublication(QDate n);
    void setDateExpiration(QDate n);

    //getters
    int getId();
    QString getTitre();
    QString getDescription();
    QDate getDatePublication();
    QDate getDateExpiration();

    //CRUD
    bool ajouter();
    QStandardItemModel * afficher();
    bool supprimer(QStandardItemModel*);
    bool modifier(QStandardItemModel*);

};

#endif // OFFRES_EMPLOI_H

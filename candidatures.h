#ifndef CANDIDATURES_H
#define CANDIDATURES_H

#include <QDialog>
#include <QStringList>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include "candidats.h"


namespace Ui {
class Candidatures;
}

class Candidatures : public QDialog
{
    Q_OBJECT

public:
    explicit Candidatures(QWidget *parent = nullptr);
    Candidatures(){}
    Candidatures(int,int,QDate,QString);
    ~Candidatures();

    //setters
    void setID(int n);
    void setDateCandidature(QDate n);
    void setStatus(QString n);

    //getters
    int getId();
    QDate getDateCandidature();
    QString getStatus();

    //CRUD
    bool ajouter();
    QStandardItemModel * afficher();
    bool supprimer(QStandardItemModel*);
    bool modifier(QStandardItemModel*);

private slots:
    void on_Ajouter_candidature_clicked();



    void on_Supprimer_candidature_clicked();

private:
    int id_candidature, id_offre,id_candidat;
    QDate date_candidature;
    QString status;
    Ui::Candidatures *ui;

};

#endif // CANDIDATURES_H

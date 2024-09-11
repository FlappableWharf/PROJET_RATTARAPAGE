#ifndef CANDIDATS_H
#define CANDIDATS_H
#include <QString>
#include <QDate>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChart>
#include <QtCore/QDebug>

class candidats
{
public:
    candidats(){}
    candidats(QString,QString,QString,QString);
private:
    int id_candidat;
    QString nom, prenom, email, telephone;
public:
    //setters
    void setID(int n);
    void setNom(QString n);
    void setPrenom(QString n);
    void setEmail(QString n);
    void setTelephone(QString n);

    //getters
    int getId();
    QString getNom();
    QString getPrenom();
    QString getEmail();
    QString getTelephone();

    //CRUD
    bool ajouter();
    QStandardItemModel * afficher();
    bool supprimer(QStandardItemModel*);
    bool modifier(QStandardItemModel*);
    QChartView* createApplicationsChart();
    void displayChartA();

};

#endif // CANDIDATS_H

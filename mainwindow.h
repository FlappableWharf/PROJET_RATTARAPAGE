#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "offres_emploi.h"
#include "modifieremploi.h"
#include "modifiercandidat.h"
#include "candidats.h"
#include "candidatures.h"
#include <QTableView>
#include <QPrinter>
#include <QFileDialog>
#include <QSortFilterProxyModel>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool isNumeric(const QString &searchText);
    ~MainWindow();

private slots:
    void on_ajouter_emploi_clicked();

    void on_Supprimer_emploi_clicked();

    void on_modifier_emploi_clicked();

    void on_ExportBtn_Emploi_clicked();

    void exportToPdf(const QString &filePath, QTableView *tableView);

    void on_comboBox_emploi_currentIndexChanged();

    void on_lineEdit_Rechercher_emploi_textChanged(const QString &searchText);

    void on_ajouter_candidat_clicked();

    void on_modifier_candidat_clicked();

    void on_supprimer_candidat_clicked();

    void on_ExportBtn_candidats_clicked();

    void on_lineEdit_Rechercher_candidats_textChanged(const QString &arg1);

    void on_comboBox_candidats_currentIndexChanged();

    void on_Candidatures_clicked();

    void on_Stats_clicked();

private:
    Ui::MainWindow *ui;
    offres_emploi Etmp;
    candidats Etmp2;
};
#endif // MAINWINDOW_H

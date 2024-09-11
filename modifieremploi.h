#ifndef MODIFIEREMPLOI_H
#define MODIFIEREMPLOI_H

#include <QDialog>
#include "offres_emploi.h"
#include <QMessageBox>
#include <QStringListModel>
#include <QSortFilterProxyModel>
namespace Ui {
class ModifierEmploi;
}

class ModifierEmploi : public QDialog {
    Q_OBJECT

public:
    explicit ModifierEmploi(QStandardItemModel *model, QWidget *parent = nullptr);
    ~ModifierEmploi();
    void setOffre(offres_emploi &s); // Change to pass by reference

private slots:
    void on_ConfirmModOffre_clicked();

private:
    Ui::ModifierEmploi *ui;
    QStandardItemModel *model;
    offres_emploi Etmp;
};

#endif // MODIFIEREMPLOI_H

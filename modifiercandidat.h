#ifndef MODIFIERCANDIDAT_H
#define MODIFIERCANDIDAT_H

#include <QDialog>
#include "candidats.h"
#include <QMessageBox>
#include "mainwindow.h"

namespace Ui {
class ModifierCandidat;
}

class ModifierCandidat : public QDialog {
    Q_OBJECT

public:
    explicit ModifierCandidat(QStandardItemModel *model, QWidget *parent = nullptr);
    ~ModifierCandidat();
    void setCandidat(candidats &s); // Change to pass by reference
    bool isNumeric(const QString &searchText);


private slots:
    void on_ConfirmerCandidat_clicked();

private:
    Ui::ModifierCandidat *ui;
    QStandardItemModel *model;
    candidats Etmp2;
};

#endif // MODIFIERCANDIDAT_H

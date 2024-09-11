#include "modifieremploi.h"
#include "ui_modifieremploi.h"
#include "offres_emploi.h"

ModifierEmploi::ModifierEmploi(QStandardItemModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifierEmploi),
    model(model)
{
    ui->setupUi(this);

    ui->listView_entreprise_Mod->setVisible(false);
    QStringListModel *model1 = new QStringListModel(this);
    QSortFilterProxyModel *proxyModel1 = new QSortFilterProxyModel(this);
    proxyModel1->setSourceModel(model1);
    ui->listView_entreprise_Mod->setModel(proxyModel1);

    QSqlQuery query1;
    QStringList items1;
    if (query1.exec("SELECT nom_entreprise, telephone FROM entreprises")) {
        while (query1.next()) {
            QString nom_entreprise = query1.value(0).toString();
            QString telephone = query1.value(1).toString();
            QString displayText = QString("%1 | %2").arg(nom_entreprise).arg(telephone);
            items1 << displayText;
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to fetch data from the database.");
    }

    model1->setStringList(items1);

    connect(ui->lineEdit_Entreprise_Mod, &QLineEdit::textEdited, this, [=](const QString &text) {
        proxyModel1->setFilterWildcard(text + "*");
        ui->listView_entreprise_Mod->setVisible(proxyModel1->rowCount() > 0);
    });

    connect(ui->listView_entreprise_Mod, &QListView::clicked, this, [=](const QModelIndex &index) {
        ui->lineEdit_Entreprise_Mod->setText(proxyModel1->data(index).toString());
        ui->listView_entreprise_Mod->setVisible(false);
    });

    connect(ui->lineEdit_Entreprise_Mod, &QLineEdit::editingFinished, this, [=]() {
        ui->listView_entreprise_Mod->setVisible(false);
    });
}

ModifierEmploi::~ModifierEmploi() {
    delete ui;
}

void ModifierEmploi::setOffre(offres_emploi &s) { // Change to pass by reference
    s.setTitre(ui->lineEdit_titreEmploi_mod->text());
    s.setDescription(ui->textEdit_Description_emploi_mod->toPlainText());
    s.setDatePublication(ui->dateEdit_Publication_Emploi_mod->date());
    s.setDateExpiration(ui->dateEdit_Expiration_emploi_mod->date());
    s.setIdEntreprise(ui->lineEdit_Entreprise_Mod->text().toInt());
}


void ModifierEmploi::on_ConfirmModOffre_clicked()
{
    QString titre = ui->lineEdit_titreEmploi_mod->text();
    QString description = ui->textEdit_Description_emploi_mod->toPlainText();
    QDate date_publication = ui->dateEdit_Publication_Emploi_mod->date();
    QDate date_expiration = ui->dateEdit_Expiration_emploi_mod->date();
    int id_entreprise = ui->lineEdit_Entreprise_Mod->text().toInt();

    offres_emploi t(titre, description, date_publication, date_expiration, id_entreprise);

    bool test = t.modifier(model);

    if(test){
        QMessageBox::information(nullptr, QObject::tr("Successful"),
                                 QObject::tr("Modification effectué\n"
                                 "Click Cancel to exit."), QMessageBox::Cancel);
        ui->lineEdit_titreEmploi_mod->clear();
        ui->textEdit_Description_emploi_mod->clear();
        ui->dateEdit_Publication_Emploi_mod->clear();
        ui->dateEdit_Expiration_emploi_mod->clear();
        ui->lineEdit_Entreprise_Mod->clear();
        close();
    }
    else{
        QMessageBox::critical(nullptr, QObject::tr("Declined"),
                             QObject::tr("Modification non effectue.\n"
                                         "Click Cancel to exit."),QMessageBox::Cancel);
        close();
    }
}

#include "modifiercandidat.h"
#include "ui_modifiercandidat.h"

ModifierCandidat::ModifierCandidat(QStandardItemModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifierCandidat),
    model(model)
{
    ui->setupUi(this);
}

ModifierCandidat::~ModifierCandidat() {
    delete ui;
}

void ModifierCandidat::setCandidat(candidats &s) { // Change to pass by reference
    s.setNom(ui->lineEdit_nom_candidat_mod->text());
    s.setPrenom(ui->lineEdit_prenom_candidat_mod->text());
    s.setEmail(ui->lineEdit_email_candidat_mod->text());
    s.setTelephone(ui->lineEdit_tel_candidat_mod->text());
}

void ModifierCandidat::on_ConfirmerCandidat_clicked()
{
    QString nom = ui->lineEdit_nom_candidat_mod->text();
    QString prenom = ui->lineEdit_prenom_candidat_mod->text();
    QString email = ui->lineEdit_email_candidat_mod->text();
    QString telephone = ui->lineEdit_tel_candidat_mod->text();

    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Le nom ne doit pas être vide.");
        return;
    }

    if (prenom.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Le prénom ne doit pas être vide.");
        return;
    }

    QRegularExpression emailRegex(QStringLiteral("^[\\w\\-\\.]+@([\\w\\-]+\\.)+[\\w\\-]{2,4}$"));

    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Input Error", "L'adresse email n'est pas valide.");
        return;
    }

    if (!isNumeric(telephone) || telephone.length() != 8) {
        QMessageBox::warning(this, "Input Error", "Le numéro de téléphone doit contenir 8 chiffres.");
        return;
    }

    candidats t(nom, prenom, email, telephone);

    bool test = t.modifier(model);

    if(test){
        QMessageBox::information(nullptr, QObject::tr("Successful"),
                                 QObject::tr("Modification effectué\n"
                                 "Click Cancel to exit."), QMessageBox::Cancel);
        ui->lineEdit_nom_candidat_mod->clear();
        ui->lineEdit_prenom_candidat_mod->clear();
        ui->lineEdit_email_candidat_mod->clear();
        ui->lineEdit_tel_candidat_mod->clear();
        close();
    }
    else{
        QMessageBox::critical(nullptr, QObject::tr("Declined"),
                             QObject::tr("Modification non effectue.\n"
                                         "Click Cancel to exit."),QMessageBox::Cancel);
        close();
    }
}

bool ModifierCandidat::isNumeric(const QString &searchText){
    bool isNumeric = true;
    for (const QChar &c : searchText) {
        if (!c.isDigit()) {
            isNumeric = false;
            break;
        }
    }
    return isNumeric;
}

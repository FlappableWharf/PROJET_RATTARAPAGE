#include "candidatures.h"
#include "ui_candidatures.h"
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

Candidatures::Candidatures(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Candidatures)
{
    ui->setupUi(this);

    ui->tableView->setModel(this->afficher());

    // Setup for the first LineEdit and ListView
    ui->listView->setVisible(false);
    QStringListModel *model1 = new QStringListModel(this);
    QSortFilterProxyModel *proxyModel1 = new QSortFilterProxyModel(this);
    proxyModel1->setSourceModel(model1);
    ui->listView->setModel(proxyModel1);

    QSqlQuery query1;
    QStringList items1;
    if (query1.exec("SELECT nom, prenom, email, telephone FROM candidats")) {
        while (query1.next()) {
            QString nom = query1.value(0).toString();
            QString prenom = query1.value(1).toString();
            QString email = query1.value(2).toString();
            QString telephone = query1.value(3).toString();
            QString displayText = QString("%1 %2 | %3 | %4").arg(nom).arg(prenom).arg(telephone).arg(email);
            items1 << displayText;
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to fetch data from the database.");
    }

    model1->setStringList(items1);

    connect(ui->lineEdit, &QLineEdit::textEdited, this, [=](const QString &text) {
        proxyModel1->setFilterWildcard(text + "*");
        ui->listView->setVisible(proxyModel1->rowCount() > 0);
    });

    connect(ui->listView, &QListView::clicked, this, [=](const QModelIndex &index) {
        ui->lineEdit->setText(proxyModel1->data(index).toString());
        ui->listView->setVisible(false);
    });

    connect(ui->lineEdit, &QLineEdit::editingFinished, this, [=]() {
        ui->listView->setVisible(false);
    });

    // Setup for the second LineEdit and ListView
    ui->listView_2->setVisible(false);
    QStringListModel *model2 = new QStringListModel(this);
    QSortFilterProxyModel *proxyModel2 = new QSortFilterProxyModel(this);
    proxyModel2->setSourceModel(model2);
    ui->listView_2->setModel(proxyModel2);

    QSqlQuery query2;
    QStringList items2;
    if (query2.exec("SELECT o.titre, o.date_publication, o.date_expiration, e.nom_entreprise "
                    "FROM offresemploi o "
                    "JOIN entreprises e ON o.id_entreprise = e.id_entreprise")) {
        while (query2.next()) {
            QString titre = query2.value(0).toString();
            QString date_publication = query2.value(1).toString();
            QString date_expiration = query2.value(2).toString();
            QString nom_entreprise = query2.value(3).toString();

            QString displayText2 = QString("%1 | %2 | %3 | %4")
                                   .arg(nom_entreprise)
                                   .arg(titre)
                                   .arg(date_publication)
                                   .arg(date_expiration);

            items2 << displayText2;
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to fetch data from the database.");
    }


    model2->setStringList(items2);

    connect(ui->lineEdit_2, &QLineEdit::textEdited, this, [=](const QString &text) {
        proxyModel2->setFilterWildcard(text + "*");
        ui->listView_2->setVisible(proxyModel2->rowCount() > 0);
    });

    connect(ui->listView_2, &QListView::clicked, this, [=](const QModelIndex &index) {
        ui->lineEdit_2->setText(proxyModel2->data(index).toString());
        ui->listView_2->setVisible(false);
    });

    connect(ui->lineEdit_2, &QLineEdit::editingFinished, this, [=]() {
        ui->listView_2->setVisible(false);
    });
}

Candidatures::~Candidatures()
{
    delete ui;
    //delete chartView;
}

Candidatures::Candidatures(int id_offre, int id_candidat, QDate date_candidature, QString status)
{
    this->id_offre = id_offre;
    this->id_candidat = id_candidat;
    this->date_candidature = date_candidature;
    this->status = status;
}

// Getters
int Candidatures::getId() {
    return id_candidature;
}

QDate Candidatures::getDateCandidature() {
    return date_candidature;
}

QString Candidatures::getStatus() {
    return status;
}

// Setters
void Candidatures::setID(int n) {
    id_candidature = n;
}

void Candidatures::setDateCandidature(QDate n) {
    date_candidature = n;
}

void Candidatures::setStatus(QString n) {
    status = n;
}

bool Candidatures::ajouter() {
    QSqlQuery query;

    query.prepare("INSERT INTO candidatures(id_offre, id_candidat, date_candidature, statut_candidature) VALUES (:id_offre, :id_candidat, :date_candidature, :status_candidature)");
    query.bindValue(":id_offre", id_offre);
    query.bindValue(":id_candidat", id_candidat);
    query.bindValue(":date_candidature", date_candidature);
    query.bindValue(":status_candidature", status);

    return query.exec();
}

QStandardItemModel* Candidatures::afficher() {
    QSqlQuery query;
    query.prepare("SELECT * FROM candidatures");
    query.exec();

    QStandardItemModel *model = new QStandardItemModel();
    model->setColumnCount(5);
    model->setHorizontalHeaderLabels(QStringList() << "ID_CANDIDATURE" << "TITRE_OFFRE" << "CANDIDAT" << "DATE CANDIDATURE" << "STATUS CANDIDATURE" << "SELECT");

    int row = 0;
    while (query.next()) {
        for (int column = 0; column < 5; ++column) {
            QString value = query.value(column).toString();
            QStandardItem *item = new QStandardItem(value);
            model->setItem(row, column, item);
        }

        QStandardItem *checkboxItem = new QStandardItem();
        checkboxItem->setCheckable(true);
        model->setItem(row, 5, checkboxItem);

        row++;
    }

    return model;
}

bool Candidatures::supprimer(QStandardItemModel *model) {
    QSqlQuery query;

    for (int row = 0; row < model->rowCount(); ++row) {
        QStandardItem *checkboxItem = model->item(row, 5);
        if (checkboxItem->checkState() == Qt::Checked) {
            QString id = model->item(row, 0)->text();
            query.prepare("DELETE FROM candidatures WHERE id_candidature = :id");
            query.bindValue(":id", id);

            if (!query.exec()) {
                return false;
            }
        }
    }

    return true;
}

bool Candidatures::modifier(QStandardItemModel *model) {
    QSqlQuery query;

    for (int row = 0; row < model->rowCount(); ++row) {
        QStandardItem *checkboxItem = model->item(row, 5);
        if (checkboxItem->checkState() == Qt::Checked) {
            QString id = model->item(row, 0)->text();

            query.prepare("UPDATE candidatures SET id_offre = :id_offre, id_candidat = :id_candidat, date_candidature = :date_candidature, statut_candidature = :status_candidature WHERE id_candidature = :id");
            query.bindValue(":id_offre", id_offre);
            query.bindValue(":id_candidat", id_candidat);
            query.bindValue(":date_candidature", date_candidature);
            query.bindValue(":status_candidature", status);
            query.bindValue(":id", id);

            if (!query.exec()) {
                return false;
            }
        }
    }

    return true;
}

void Candidatures::on_Ajouter_candidature_clicked()
{
    QString candidatInfo = ui->lineEdit->text();
    QString offreInfo = ui->lineEdit_2->text();

    if (candidatInfo.isEmpty() || offreInfo.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select both a candidate and a job offer.");
        return;
    }

    QStringList candidatDetails = candidatInfo.split(" | ");
    QString telephone = candidatDetails[1].trimmed();
    QString email = candidatDetails[2].trimmed(); // Get candidate's email

    QStringList offreDetails = offreInfo.split(" | ");
    QString titre = offreDetails[1].trimmed();
    QString nom_entreprise = offreDetails[0].trimmed();

    QSqlQuery query;
    int id_candidat = -1;
    query.prepare("SELECT id_candidat FROM candidats WHERE email = :email OR telephone = :telephone");
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);

    if (query.exec()) {
        if (query.next()) {
            id_candidat = query.value(0).toInt();
        }
    } else {
        qDebug() << "Error retrieving candidate ID:" << query.lastError().text();
    }

    if (id_candidat == -1) {
        QMessageBox::critical(this, "Error", "Failed to retrieve candidate ID.");
        return;
    }

    int id_offre = -1;
    query.prepare("SELECT o.id_offre FROM offresemploi o JOIN entreprises e ON o.id_entreprise = e.id_entreprise "
                  "WHERE o.titre = :titre AND e.nom_entreprise = :nom_entreprise");
    query.bindValue(":titre", titre);
    query.bindValue(":nom_entreprise", nom_entreprise);

    if (query.exec()) {
        if (query.next()) {
            id_offre = query.value(0).toInt();
        }
    } else {
        qDebug() << "Error retrieving job offer ID:" << query.lastError().text();
    }

    if (id_offre == -1) {
        QMessageBox::critical(this, "Error", "Failed to retrieve job offer ID.");
        return;
    }

    qDebug() << "Candidate ID:" << id_candidat;
    qDebug() << "Offer ID:" << id_offre;

    QDate date_candidature = ui->dateTimeEdit->date();
    QString status = ui->textEditStatus->toPlainText();

    Candidatures candidature(id_offre, id_candidat, date_candidature, status);
    if (candidature.ajouter()) {
        QMessageBox::information(this, "Success", "Candidature added successfully.");
        ui->tableView->setModel(this->afficher());

        // Send email to candidate
        QString subject = "New Job Application Confirmation";
        QString body = QString("Dear %1,\n\nThank you for applying to the position of %2 at %3.\n\nBest regards,\nYour Company")
                        .arg(candidatDetails[0]) // Candidate's name
                        .arg(titre) // Job title
                        .arg(nom_entreprise); // Company name

        QUrl mailtoUrl = QUrl(QString("mailto:%1?subject=%2&body=%3")
                               .arg(email)
                               .arg(subject)
                               .arg(body));

        QDesktopServices::openUrl(mailtoUrl); // Open the default email client with pre-filled email details

    } else {
        QMessageBox::critical(this, "Error", "Failed to add candidature.");
    }

    // Clear the input fields
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->textEditStatus->clear();
    ui->dateTimeEdit->clear();
}


void Candidatures::on_Supprimer_candidature_clicked() {
    // Get the model from the tableView
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableView->model());

    // Call the supprimer method to delete the selected candidatures
    if (this->supprimer(model)) {
        // Refresh the tableView with the updated model after deletion
        ui->tableView->setModel(this->afficher());
    } else {
        // Show an error message if deletion fails
        QMessageBox::critical(this, "Error", "Failed to delete candidature(s).");
    }
}

#include "offres_emploi.h"

offres_emploi::offres_emploi(QString titre,QString description,QDate date_publication,QDate date_expiration, int id_entreprise)
{
    this->titre=titre;
    this->description=description;
    this->date_expiration=date_expiration;
    this->date_publication=date_publication;
    this->id_entreprise=id_entreprise;
}

//getters
int offres_emploi::getId(){
    return id_offre;
}
QString offres_emploi::getTitre(){
    return titre;
}
QString offres_emploi::getDescription(){
    return description;
}
QDate offres_emploi::getDateExpiration(){
    return date_expiration;
}
QDate offres_emploi::getDatePublication(){
    return date_publication;
}

//setters
void offres_emploi::setID(int n){
    id_offre=n;
}
void offres_emploi::setIdEntreprise(int n){
    id_entreprise=n;
}
void offres_emploi::setTitre(QString n){
    titre=n;
}
void offres_emploi::setDescription(QString n){
    description=n;
}
void offres_emploi::setDateExpiration(QDate n){
    date_expiration=n;
}
void offres_emploi::setDatePublication(QDate n){
    date_publication=n;
}

bool offres_emploi::ajouter() {
    QSqlQuery query;

    query.prepare("INSERT INTO OFFRESEMPLOI(titre, description, date_publication, date_expiration,id_entreprise) VALUES (:titre, :description, :date_publication, :date_expiration, :id_entreprise)");
    query.bindValue(":titre", titre);
    query.bindValue(":description", description);
    query.bindValue(":date_publication", date_publication);
    query.bindValue(":date_expiration", date_expiration);
    query.bindValue(":id_entreprise", id_entreprise);

    return query.exec();
}

QStandardItemModel * offres_emploi::afficher(){
    QSqlQuery query;
    query.prepare("select * from offresemploi");
    query.exec();

    QStandardItemModel *model = new QStandardItemModel();

    model->setColumnCount(6);
    model->setHorizontalHeaderLabels(QStringList() << "ID_EMPLOI" << "TITRE" << "DESCRIPTION" << "DATE_PUBLICATION" << "DATE_EXPIRATION" << "ID_ENTREPRISE" << "SELECT");

    int row = 0;
    while(query.next()) {
        for(int column = 0; column < 6; ++column) {
            QString value = query.value(column).toString();
            QStandardItem *item = new QStandardItem(value);
            model->setItem(row, column, item);
        }

        // Add a checkbox as the last column
        QStandardItem *checkboxItem = new QStandardItem();
        checkboxItem->setCheckable(true);
        model->setItem(row, 6, checkboxItem);

        row++;
    }

    return model;
}

bool offres_emploi::supprimer(QStandardItemModel *model){
    QSqlQuery query;

    for(int row = 0; row < model->rowCount(); ++row) {

        QStandardItem *checkboxItem = model->item(row, 6);
        if(checkboxItem->checkState() == Qt::Checked) {

            QString id = model->item(row, 0)->text();

            query.prepare("delete from offresemploi where id_offre = :id");
            query.bindValue(":id", id);


            if(!query.exec()) {
                // If query execution fails, return false
                return false;
            }
        }
    }

    // If all deletions were successful, return true
    return true;
}

bool offres_emploi::modifier(QStandardItemModel *model){
    QSqlQuery query;

    for(int row = 0; row < model->rowCount(); ++row) {
    QStandardItem *checkboxItem = model->item(row, 6);
    if(checkboxItem->checkState() == Qt::Checked){
    QString id = model->item(row, 0)->text();

    // Update the patient's information in the database
    query.prepare("UPDATE offresemploi SET titre = :titre, description = :description, date_publication = :date_publication, date_expiration = :date_expiration, id_entreprise = :id_entreprise WHERE id_offre = :id");
    query.bindValue(":titre", titre);
    query.bindValue(":description", description);
    query.bindValue(":date_publication", date_publication);
    query.bindValue(":date_expiration", date_expiration);
    query.bindValue(":id_entreprise", id_entreprise);
    query.bindValue(":id", id);

    }
    // Execute the update query
    }
    return query.exec();
}

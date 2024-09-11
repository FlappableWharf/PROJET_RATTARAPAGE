#include "candidats.h"

candidats::candidats(QString nom,QString prenom,QString email, QString telephone)
{
    this->nom=nom;
    this->prenom=prenom;
    this->email=email;
    this->telephone=telephone;
}

//getters
int candidats::getId(){
    return id_candidat;
}
QString candidats::getNom(){
    return nom;
}
QString candidats::getPrenom(){
    return prenom;
}
QString candidats::getEmail(){
    return email;
}
QString candidats::getTelephone(){
    return telephone;
}

//setters
void candidats::setID(int n){
    id_candidat=n;
}
void candidats::setNom(QString n){
    nom=n;
}
void candidats::setPrenom(QString n){
    prenom=n;
}
void candidats::setEmail(QString n){
    email=n;
}
void candidats::setTelephone(QString n){
    telephone=n;
}

bool candidats::ajouter() {
    QSqlQuery query;

    query.prepare("INSERT INTO candidats(nom, prenom, email, telephone) VALUES (:nom, :prenom, :email, :telephone)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);

    return query.exec();
}

QStandardItemModel * candidats::afficher(){
    QSqlQuery query;
    query.prepare("select * from candidats");
    query.exec();

    QStandardItemModel *model = new QStandardItemModel();

    model->setColumnCount(5);
    model->setHorizontalHeaderLabels(QStringList() << "ID_CANDIDAT" << "NOM" << "PRENOM" << "EMAIL" << "TELEPHONE" << "SELECT");

    int row = 0;
    while(query.next()) {
        for(int column = 0; column < 5; ++column) {
            QString value = query.value(column).toString();
            QStandardItem *item = new QStandardItem(value);
            model->setItem(row, column, item);
        }

        // Add a checkbox as the last column
        QStandardItem *checkboxItem = new QStandardItem();
        checkboxItem->setCheckable(true);
        model->setItem(row, 5, checkboxItem);

        row++;
    }

    return model;
}

bool candidats::supprimer(QStandardItemModel *model){
    QSqlQuery query;

    for(int row = 0; row < model->rowCount(); ++row) {

        QStandardItem *checkboxItem = model->item(row, 5);
        if(checkboxItem->checkState() == Qt::Checked) {

            QString id = model->item(row, 0)->text();

            query.prepare("delete from candidats where id_candidat = :id");
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

bool candidats::modifier(QStandardItemModel *model){
    QSqlQuery query;

    for(int row = 0; row < model->rowCount(); ++row) {
    QStandardItem *checkboxItem = model->item(row, 5);
    if(checkboxItem->checkState() == Qt::Checked){
    QString id = model->item(row, 0)->text();

    // Update the patient's information in the database
    query.prepare("UPDATE candidats SET nom = :nom, prenom = :prenom, email = :email, telephone = :telephone WHERE id_candidat = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":id", id);

    }
    // Execute the update query
    }
    return query.exec();
}

QChartView* candidats::createApplicationsChart() {
    // Query the database
    QSqlQuery query;
    query.prepare("SELECT id_offre, COUNT(*) as num_applications FROM CANDIDATURES GROUP BY id_offre");
    query.exec();

    // Prepare data for the chart
    QBarSet *set = new QBarSet("Applications");
    QStringList categories;

    while (query.next()) {
        QString idOffre = query.value(0).toString();
        int numApplications = query.value(1).toInt();

        *set << numApplications;
        categories << idOffre;
    }

    // Create a bar series and add the set to it
    QBarSeries *series = new QBarSeries();
    series->append(set);

    // Create a chart and set the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Applications per Job Offer");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create category axis
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Create value axis
    QValueAxis *axisY = new QValueAxis();

    // Compute the maximum value for Y-axis
    int maxValue = 0;
    for (auto barSet : series->barSets()) {
        for (int i = 0; i < barSet->count(); ++i) {
            int value = barSet->at(i);
            if (value > maxValue) {
                maxValue = value;
            }
        }
    }

    axisY->setRange(0, maxValue);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Create a chart view and set the chart
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    return chartView;
}

void candidats::displayChartA(){
    QChartView *chartView = createApplicationsChart();
    chartView->resize(1500,800);
    chartView->show();
}



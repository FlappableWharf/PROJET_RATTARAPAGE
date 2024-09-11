#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView_emploi->setModel(Etmp.afficher());
    ui->tableView_candidats->setModel(Etmp2.afficher());

    ui->listView_entreprise->setVisible(false);
    QStringListModel *model1 = new QStringListModel(this);
    QSortFilterProxyModel *proxyModel1 = new QSortFilterProxyModel(this);
    proxyModel1->setSourceModel(model1);
    ui->listView_entreprise->setModel(proxyModel1);

    QSqlQuery query1;
    QStringList items1;
    if (query1.exec("SELECT id_entreprise FROM entreprises")) {
        while (query1.next()) {
            QString id_entreprise = query1.value(0).toString();
            //QString telephone = query1.value(1).toString();
            QString displayText = QString("%1").arg(id_entreprise);
            items1 << displayText;
        }
    } else {
        QMessageBox::critical(this, "Error", "Failed to fetch data from the database.");
    }

    model1->setStringList(items1);

    connect(ui->lineEdit_Entreprise, &QLineEdit::textEdited, this, [=](const QString &text) {
        proxyModel1->setFilterWildcard(text + "*");
        ui->listView_entreprise->setVisible(proxyModel1->rowCount() > 0);
    });

    connect(ui->listView_entreprise, &QListView::clicked, this, [=](const QModelIndex &index) {
        ui->lineEdit_Entreprise->setText(proxyModel1->data(index).toString());
        ui->listView_entreprise->setVisible(false);
    });

    connect(ui->lineEdit_Entreprise, &QLineEdit::editingFinished, this, [=]() {
        ui->listView_entreprise->setVisible(false);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ajouter_emploi_clicked()
{
    QString titre = ui->lineEdit_titreEmploi->text();
    QString description = ui->textEdit_Description_emploi->toPlainText();
    QDate date_expiration = ui->dateEdit_Expiration_emploi->date();
    QDate date_publication = ui->dateEdit_Publication_Emploi->date();
    int id_entreprise = ui->lineEdit_Entreprise->text().toInt();

    // Validation checks
    if (titre.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Le titre ne doit pas être vide.");
        return;
    }

    if (description.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "La description ne doit pas être vide.");
        return;
    }

    if (date_expiration <= date_publication) {
        QMessageBox::warning(this, "Input Error", "La date d'expiration doit être après la date de publication.");
        return;
    }

    offres_emploi t(titre, description, date_publication, date_expiration, id_entreprise);

    bool test = t.ajouter();

    if(test) {
        ui->tableView_emploi->setModel(Etmp.afficher());

        QMessageBox::information(nullptr, QObject::tr("Successful"),
                                 QObject::tr("Ajout effectué\n"
                                 "Click Cancel to exit."), QMessageBox::Cancel);

        ui->lineEdit_titreEmploi->clear();
        ui->textEdit_Description_emploi->clear();
        ui->dateEdit_Expiration_emploi->clear();
        ui->dateEdit_Publication_Emploi->clear();
        ui->lineEdit_Entreprise->clear();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Declined"),
                             QObject::tr("Ajout non effectué.\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_Supprimer_emploi_clicked()
{
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableView_emploi->model());

    if(Etmp.supprimer(model)) {
        ui->tableView_emploi->setModel(Etmp.afficher());
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete emploi(s).");
    }
}

void MainWindow::on_modifier_emploi_clicked()
{
    QAbstractItemModel *model = ui->tableView_emploi->model();

    int checkedCount = 0;

    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, 6);

        if (model->data(index, Qt::CheckStateRole) == Qt::Checked) {
            checkedCount++;
        }
    }

    if (checkedCount == 1) {
        ModifierEmploi d(static_cast<QStandardItemModel*>(model), this);
        d.exec();
    } else {
        QMessageBox::critical(this, "Error", "Please check exactly one row to modify.");
    }
    ui->tableView_emploi->setModel(Etmp.afficher());
}

void MainWindow::exportToPdf(const QString &filePath, QTableView *tableView) {
    // Create a QTextDocument to hold the contents of the table
    QTextDocument doc;

    // Iterate over the table and append its contents to the QTextDocument
    QTextCursor cursor(&doc);
    QString tableContents;
    for (int row = 0; row < tableView->model()->rowCount(); ++row) {
        for (int column = 0; column < tableView->model()->columnCount(); ++column) {
            QModelIndex index = tableView->model()->index(row, column);
            QString data = tableView->model()->data(index).toString();
            tableContents.append(data + "\t");
        }
        tableContents.append("\n");
    }
    cursor.insertText(tableContents);

    // Create a QPrinter for PDF output
    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    // Print the QTextDocument to the PDF file
    doc.print(&printer);
}

void MainWindow::on_ExportBtn_Emploi_clicked()
{
    // Get the file path from the user
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (filePath.isEmpty())
        return; // User canceled or no file selected

    // Export the contents of the table to the PDF file
    exportToPdf(filePath, ui->tableView_emploi);
    QMessageBox::information(this, tr("Export Successful"), tr("PDF exported successfully."));
}

void MainWindow::on_comboBox_emploi_currentIndexChanged()
{
    QString sortOption = ui->comboBox_emploi->currentText();

    if (sortOption == "Alphabet (Titre) ↑") {
        ui->tableView_emploi->sortByColumn(1, Qt::AscendingOrder);
    } else if (sortOption == "Alphabet (Titre) ↓") {
        ui->tableView_emploi->sortByColumn(1, Qt::DescendingOrder);
    } else if (sortOption == "Date de Publication ↑") {
        ui->tableView_emploi->sortByColumn(3, Qt::AscendingOrder);
    } else if (sortOption == "Date de Publication ↓") {
        ui->tableView_emploi->sortByColumn(3, Qt::DescendingOrder);
    } else if (sortOption == "Date d'Expiration ↑"){
        ui->tableView_emploi->sortByColumn(4, Qt::AscendingOrder);
    } else if (sortOption == "Date d'Expiration ↓"){
        ui->tableView_emploi->sortByColumn(4, Qt::DescendingOrder);
    } else if (sortOption == "ID ↑"){
        ui->tableView_emploi->sortByColumn(0, Qt::AscendingOrder);
    } else if (sortOption == "ID ↓"){
        ui->tableView_emploi->sortByColumn(0, Qt::DescendingOrder);
    } else {
        ui->tableView_emploi->setModel(Etmp.afficher());
    }
}

void MainWindow::on_lineEdit_Rechercher_emploi_textChanged(const QString &searchText)
{
    QAbstractItemModel *model = Etmp.afficher();

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    if (searchText.isEmpty()) {
        proxyModel->setFilterRegExp(QRegExp());
    } else {
        if (isNumeric(searchText)) {
            QRegExp regex(searchText, Qt::CaseInsensitive, QRegExp::FixedString);
            proxyModel->setFilterRegExp(regex);
            proxyModel->setFilterKeyColumn(0); // ID column
        } else {
            QRegExp regex(searchText, Qt::CaseInsensitive, QRegExp::FixedString);
            proxyModel->setFilterRegExp(regex);
            proxyModel->setFilterKeyColumn(1); // Title column
        }
    }
    ui->tableView_emploi->setModel(proxyModel);
}

bool MainWindow::isNumeric(const QString &searchText){
    bool isNumeric = true;
    for (const QChar &c : searchText) {
        if (!c.isDigit()) {
            isNumeric = false;
            break;
        }
    }
    return isNumeric;
}

void MainWindow::on_ajouter_candidat_clicked()
{
    QString nom = ui->lineEdit_nom_candidat->text();
    QString prenom = ui->lineEdit_prenom_candidat->text();
    QString email = ui->lineEdit_email_candidat->text();
    QString telephone = ui->lineEdit_tel_candidat->text();

    // Validation checks
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

    candidats c(nom, prenom, email, telephone);

    bool test = c.ajouter();

    if(test) {
        ui->tableView_candidats->setModel(Etmp2.afficher());

        QMessageBox::information(nullptr, QObject::tr("Successful"),
                                 QObject::tr("Ajout effectué\n"
                                 "Click Cancel to exit."), QMessageBox::Cancel);

        ui->lineEdit_nom_candidat->clear();
        ui->lineEdit_prenom_candidat->clear();
        ui->lineEdit_email_candidat->clear();
        ui->lineEdit_tel_candidat->clear();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Declined"),
                             QObject::tr("Ajout non effectué.\n"
                                         "Click Cancel to exit."), QMessageBox::Cancel);
    }
}

void MainWindow::on_supprimer_candidat_clicked()
{
    QStandardItemModel *model = static_cast<QStandardItemModel *>(ui->tableView_candidats->model());

    if(Etmp2.supprimer(model)) {
        ui->tableView_candidats->setModel(Etmp2.afficher());
    } else {
        QMessageBox::critical(this, "Error", "Failed to delete candidat(s).");
    }
}

void MainWindow::on_modifier_candidat_clicked()
{
    QAbstractItemModel *model = ui->tableView_candidats->model();

    int checkedCount = 0;

    for (int row = 0; row < model->rowCount(); ++row) {
        QModelIndex index = model->index(row, 5);

        if (model->data(index, Qt::CheckStateRole) == Qt::Checked) {
            checkedCount++;
        }
    }

    if (checkedCount == 1) {
        ModifierCandidat d(static_cast<QStandardItemModel*>(model), this);
        d.exec();
    } else {
        QMessageBox::critical(this, "Error", "Please check exactly one row to modify.");
    }
    ui->tableView_candidats->setModel(Etmp2.afficher());
}

void MainWindow::on_ExportBtn_candidats_clicked()
{
    // Get the file path from the user
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save PDF"), "", tr("PDF Files (*.pdf)"));
    if (filePath.isEmpty())
        return; // User canceled or no file selected

    // Export the contents of the table to the PDF file
    exportToPdf(filePath, ui->tableView_candidats);
    QMessageBox::information(this, tr("Export Successful"), tr("PDF exported successfully."));
}

void MainWindow::on_comboBox_candidats_currentIndexChanged()
{
    QString sortOption = ui->comboBox_candidats->currentText();

    if (sortOption == "Alphabet (Nom) ↑") {
        ui->tableView_candidats->sortByColumn(1, Qt::AscendingOrder);
    } else if (sortOption == "Alphabet (Nom) ↓") {
        ui->tableView_candidats->sortByColumn(1, Qt::DescendingOrder);
    } else if (sortOption == "ID ↑") {
        ui->tableView_candidats->sortByColumn(0, Qt::AscendingOrder);
    } else if (sortOption == "ID ↓") {
        ui->tableView_candidats->sortByColumn(0, Qt::DescendingOrder);
    } else {
        ui->tableView_candidats->setModel(Etmp2.afficher());
    }
}

void MainWindow::on_lineEdit_Rechercher_candidats_textChanged(const QString &searchText)
{
    QAbstractItemModel *model = Etmp2.afficher();

    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    if (searchText.isEmpty()) {
        proxyModel->setFilterRegExp(QRegExp());
    } else {
        if (isNumeric(searchText)) {
            QRegExp regex(searchText, Qt::CaseInsensitive, QRegExp::FixedString);
            proxyModel->setFilterRegExp(regex);
            proxyModel->setFilterKeyColumn(0); // ID column
        } else {
            QRegExp regex(searchText, Qt::CaseInsensitive, QRegExp::FixedString);
            proxyModel->setFilterRegExp(regex);
            proxyModel->setFilterKeyColumn(1); // Nom column
        }
    }
    ui->tableView_candidats->setModel(proxyModel);
}


void MainWindow::on_Candidatures_clicked()
{
    Candidatures dialog(this);
        dialog.resize(1920,1080);
        dialog.exec();
}

void MainWindow::on_Stats_clicked()
{
    Etmp2.displayChartA();
}

#include "adminclubpage.h"
#include "ui_adminclubpage.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

AdminClubPage::AdminClubPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::AdminClubPage)
{
    ui->setupUi(this);

    proxyModel = new QSortFilterProxyModel(this);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);

    connect(ui->addButton, &QPushButton::clicked, this, &AdminClubPage::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &AdminClubPage::onEditButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AdminClubPage::onDeleteButtonClicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &AdminClubPage::onExportButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &AdminClubPage::onSearchButtonClicked);
    connect(ui->tableView, &QTableView::clicked, this, &AdminClubPage::onRowSelected);

    loadAllClubs();
}

AdminClubPage::~AdminClubPage()
{
    delete ui;
}

void AdminClubPage::loadAllClubs(const QString &filter)
{
    QString queryStr = R"(
        SELECT club_id AS ID,
               name AS Название,
               city AS Город,
               logo_path AS Логотип
        FROM clubs
    )";

    if (!filter.isEmpty()) {
        queryStr += " WHERE name ILIKE :filter ";
    }

    queryStr += " ORDER BY club_id";

    QSqlQuery query;
    query.prepare(queryStr);

    if (!filter.isEmpty()) {
        query.bindValue(":filter", "%" + filter + "%");
    }

    if (!query.exec()) {
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);
    proxyModel->setSourceModel(model);
    proxyModel->sort(0, Qt::AscendingOrder);
    ui->tableView->resizeColumnsToContents();
}

void AdminClubPage::onAddButtonClicked()
{
    QString name = ui->nameEdit->text().trimmed();
    QString city = ui->cityEdit->text().trimmed();
    QString logo = ui->logoEdit->text().trimmed();

    if (name.isEmpty() || city.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название и город клуба должны быть заполнены!");
        return;
    }

    // Получение нового ID для клуба
    QSqlQuery idQuery;
    idQuery.prepare("SELECT COALESCE(MAX(club_id), 0) + 1 FROM clubs");
    if (!idQuery.exec() || !idQuery.next()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить новый ID клуба!\n" + idQuery.lastError().text());
        return;
    }

    int newClubId = idQuery.value(0).toInt();

    // Вставка нового клуба
    QSqlQuery query;
    query.prepare("INSERT INTO clubs (club_id, name, city, logo_path) VALUES (:club_id, :name, :city, :logo)");
    query.bindValue(":club_id", newClubId);
    query.bindValue(":name", name);
    query.bindValue(":city", city);
    query.bindValue(":logo", logo);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить клуб!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Клуб успешно добавлен!");
        loadAllClubs();
    }
}

void AdminClubPage::onEditButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите клуб для редактирования!");
        return;
    }

    int clubId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();
    QString name = ui->nameEdit->text().trimmed();
    QString city = ui->cityEdit->text().trimmed();
    QString logo = ui->logoEdit->text().trimmed();

    if (name.isEmpty() || city.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Название и город клуба должны быть заполнены!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE clubs SET name = :name, city = :city, logo_path = :logo WHERE club_id = :id");
    query.bindValue(":name", name);
    query.bindValue(":city", city);
    query.bindValue(":logo", logo);
    query.bindValue(":id", clubId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные клуба!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Данные клуба успешно обновлены!");
        loadAllClubs();
    }
}

void AdminClubPage::onDeleteButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите клуб для удаления!");
        return;
    }

    int clubId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM clubs WHERE club_id = :id");
    query.bindValue(":id", clubId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить клуб!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Клуб успешно удален!");
        loadAllClubs();
    }
}

void AdminClubPage::onExportButtonClicked()
{
    exportToCSV(proxyModel, "Клубы.csv");
}

void AdminClubPage::onSearchButtonClicked()
{
    QString search = ui->searchEdit->text().trimmed();
    loadAllClubs(search);
}

void AdminClubPage::onRowSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QString name = proxyModel->data(proxyModel->index(index.row(), 1)).toString();
    QString city = proxyModel->data(proxyModel->index(index.row(), 2)).toString();
    QString logo = proxyModel->data(proxyModel->index(index.row(), 3)).toString();

    ui->nameEdit->setText(name);
    ui->cityEdit->setText(city);
    ui->logoEdit->setText(logo);
}
#include "admintournamentpage.h"
#include "ui_admintournamentpage.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

AdminTournamentPage::AdminTournamentPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::AdminTournamentPage)
{
    ui->setupUi(this);

    proxyModel = new QSortFilterProxyModel(this);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);

    connect(ui->addButton, &QPushButton::clicked, this, &AdminTournamentPage::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &AdminTournamentPage::onEditButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AdminTournamentPage::onDeleteButtonClicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &AdminTournamentPage::onExportButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &AdminTournamentPage::onSearchButtonClicked);
    connect(ui->tableView, &QTableView::clicked, this, &AdminTournamentPage::onRowSelected);

    loadAllTournaments();
}

AdminTournamentPage::~AdminTournamentPage()
{
    delete ui;
}

void AdminTournamentPage::loadAllTournaments(const QString &filter)
{
    QString queryStr = R"(
        SELECT tournament_id AS ID,
               name AS Название,
               year AS Год,
               country AS Страна
        FROM tournaments
    )";

    if (!filter.isEmpty()) {
        queryStr += " WHERE name ILIKE :filter ";
    }

    queryStr += " ORDER BY tournament_id";

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

void AdminTournamentPage::onAddButtonClicked()
{
    QString name = ui->nameEdit->text().trimmed();
    int year = ui->yearEdit->text().toInt();
    QString country = ui->countryEdit->text().trimmed();

    if (name.isEmpty() || year <= 0) {
        QMessageBox::warning(this, "Ошибка", "Название и год турнира должны быть заполнены!");
        return;
    }

    // Получение нового ID для турнира
    QSqlQuery idQuery;
    idQuery.prepare("SELECT COALESCE(MAX(tournament_id), 0) + 1 FROM tournaments");
    if (!idQuery.exec() || !idQuery.next()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить новый ID турнира!\n" + idQuery.lastError().text());
        return;
    }

    int newTournamentId = idQuery.value(0).toInt();

    // Вставка нового турнира
    QSqlQuery query;
    query.prepare("INSERT INTO tournaments (tournament_id, name, year, country) VALUES (:id, :name, :year, :country)");
    query.bindValue(":id", newTournamentId);
    query.bindValue(":name", name);
    query.bindValue(":year", year);
    query.bindValue(":country", country);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить турнир!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Турнир успешно добавлен!");
        loadAllTournaments();
    }
}

void AdminTournamentPage::onEditButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите турнир для редактирования!");
        return;
    }

    int tournamentId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();
    QString name = ui->nameEdit->text().trimmed();
    int year = ui->yearEdit->text().toInt();
    QString country = ui->countryEdit->text().trimmed();

    if (name.isEmpty() || year <= 0) {
        QMessageBox::warning(this, "Ошибка", "Название и год турнира должны быть заполнены!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE tournaments SET name = :name, year = :year, country = :country WHERE tournament_id = :id");
    query.bindValue(":name", name);
    query.bindValue(":year", year);
    query.bindValue(":country", country);
    query.bindValue(":id", tournamentId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные турнира!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Данные турнира успешно обновлены!");
        loadAllTournaments();
    }
}

void AdminTournamentPage::onDeleteButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите турнир для удаления!");
        return;
    }

    int tournamentId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM tournaments WHERE tournament_id = :id");
    query.bindValue(":id", tournamentId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить турнир!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Турнир успешно удален!");
        loadAllTournaments();
    }
}

void AdminTournamentPage::onExportButtonClicked()
{
    exportToCSV(proxyModel, "Турниры.csv");
}

void AdminTournamentPage::onSearchButtonClicked()
{
    QString search = ui->searchEdit->text().trimmed();
    loadAllTournaments(search);
}

void AdminTournamentPage::onRowSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QString name = proxyModel->data(proxyModel->index(index.row(), 1)).toString();
    int year = proxyModel->data(proxyModel->index(index.row(), 2)).toInt();
    QString country = proxyModel->data(proxyModel->index(index.row(), 3)).toString();

    ui->nameEdit->setText(name);
    ui->yearEdit->setText(QString::number(year));
    ui->countryEdit->setText(country);
}
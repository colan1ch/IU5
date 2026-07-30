#include "adminmatchpage.h"
#include "ui_adminmatchpage.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

AdminMatchPage::AdminMatchPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::AdminMatchPage)
{
    ui->setupUi(this);

    proxyModel = new QSortFilterProxyModel(this);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);

    connect(ui->addButton, &QPushButton::clicked, this, &AdminMatchPage::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &AdminMatchPage::onEditButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AdminMatchPage::onDeleteButtonClicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &AdminMatchPage::onExportButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &AdminMatchPage::onSearchButtonClicked);
    connect(ui->tableView, &QTableView::clicked, this, &AdminMatchPage::onRowSelected);

    loadClubs();
    loadTournaments();
    loadAllMatches();
}

AdminMatchPage::~AdminMatchPage()
{
    delete ui;
}

void AdminMatchPage::loadAllMatches(const QString &filter)
{
    QString queryStr = R"(
        SELECT match_id AS ID,
               match_date AS "Дата матча",
               hc.name AS "Домашний клуб",
               score AS "Счет",
               ac.name AS "Гостевой клуб",
               t.name AS Турнир
        FROM matches
        LEFT JOIN clubs hc ON matches.home_club_id = hc.club_id
        LEFT JOIN clubs ac ON matches.away_club_id = ac.club_id
        LEFT JOIN tournaments t ON matches.tournament_id = t.tournament_id
    )";

    if (!filter.isEmpty()) {
        queryStr += " WHERE hc.name ILIKE :filter OR ac.name ILIKE :filter OR t.name ILIKE :filter ";
    }

    queryStr += " ORDER BY match_id";

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

void AdminMatchPage::onAddButtonClicked()
{
    QString matchDate = ui->dateEdit->text().trimmed();
    int homeClubId = ui->homeClubComboBox->currentData().toInt();
    int awayClubId = ui->awayClubComboBox->currentData().toInt();
    QString score = ui->scoreEdit->text().trimmed();
    int tournamentId = ui->tournamentComboBox->currentData().toInt();

    if (matchDate.isEmpty() || homeClubId == 0 || awayClubId == 0 || score.isEmpty() || tournamentId == 0) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    QSqlQuery idQuery;
    idQuery.prepare("SELECT COALESCE(MAX(match_id), 0) + 1 FROM matches");
    if (!idQuery.exec() || !idQuery.next()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить новый ID матча!\n" + idQuery.lastError().text());
        return;
    }

    int newMatchId = idQuery.value(0).toInt();

    QSqlQuery query;
    query.prepare("INSERT INTO matches (match_id, match_date, home_club_id, away_club_id, score, tournament_id) "
                  "VALUES (:match_id, :match_date, :home_club_id, :away_club_id, :score, :tournament_id)");
    query.bindValue(":match_id", newMatchId);
    query.bindValue(":match_date", matchDate);
    query.bindValue(":home_club_id", homeClubId);
    query.bindValue(":away_club_id", awayClubId);
    query.bindValue(":score", score);
    query.bindValue(":tournament_id", tournamentId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить матч!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Матч успешно добавлен!");
        loadAllMatches();
    }
}

void AdminMatchPage::onEditButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите матч для редактирования!");
        return;
    }

    int matchId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();
    QString matchDate = ui->dateEdit->text().trimmed();
    int homeClubId = ui->homeClubComboBox->currentData().toInt();
    int awayClubId = ui->awayClubComboBox->currentData().toInt();
    QString score = ui->scoreEdit->text().trimmed();
    int tournamentId = ui->tournamentComboBox->currentData().toInt();

    if (matchDate.isEmpty() || homeClubId == 0 || awayClubId == 0 || score.isEmpty() || tournamentId == 0) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE matches SET match_date = :match_date, home_club_id = :home_club_id, "
                  "away_club_id = :away_club_id, score = :score, tournament_id = :tournament_id "
                  "WHERE match_id = :match_id");
    query.bindValue(":match_date", matchDate);
    query.bindValue(":home_club_id", homeClubId);
    query.bindValue(":away_club_id", awayClubId);
    query.bindValue(":score", score);
    query.bindValue(":tournament_id", tournamentId);
    query.bindValue(":match_id", matchId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные матча!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Данные матча успешно обновлены!");
        loadAllMatches();
    }
}

void AdminMatchPage::onDeleteButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите матч для удаления!");
        return;
    }

    int matchId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM matches WHERE match_id = :id");
    query.bindValue(":id", matchId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить матч!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Матч успешно удален!");
        loadAllMatches();
    }
}

void AdminMatchPage::onExportButtonClicked()
{
    exportToCSV(proxyModel, "Матчи.csv");
}

void AdminMatchPage::onSearchButtonClicked()
{
    QString search = ui->searchEdit->text().trimmed();
    loadAllMatches(search);
}

void AdminMatchPage::onRowSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    // Преобразуем индекс из proxyModel в исходный индекс модели
    QModelIndex sourceIndex = proxyModel->mapToSource(index);

    QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(proxyModel->sourceModel());
    if (!model) return;

    // Извлекаем данные из исходной модели
    QString matchDate = model->data(model->index(sourceIndex.row(), 1)).toString();
    ui->dateEdit->setDate(QDate::fromString(matchDate, "yyyy-MM-dd")); // Устанавливаем дату в QDateEdit

    QString homeClub = model->data(model->index(sourceIndex.row(), 2)).toString();
    int homeClubIndex = ui->homeClubComboBox->findText(homeClub);
    if (homeClubIndex != -1) {
        ui->homeClubComboBox->setCurrentIndex(homeClubIndex);
    }

    QString awayClub = model->data(model->index(sourceIndex.row(), 4)).toString();
    int awayClubIndex = ui->awayClubComboBox->findText(awayClub);
    if (awayClubIndex != -1) {
        ui->awayClubComboBox->setCurrentIndex(awayClubIndex);
    }

    QString score = model->data(model->index(sourceIndex.row(), 3)).toString();
    ui->scoreEdit->setText(score);

    QString tournament = model->data(model->index(sourceIndex.row(), 5)).toString();
    int tournamentIndex = ui->tournamentComboBox->findText(tournament);
    if (tournamentIndex != -1) {
        ui->tournamentComboBox->setCurrentIndex(tournamentIndex);
    }
}

void AdminMatchPage::loadClubs()
{
    ui->homeClubComboBox->clear();
    ui->awayClubComboBox->clear();

    QSqlQuery query;
    query.prepare("SELECT club_id, name FROM clubs ORDER BY name"); // Используем prepare()

    if (!query.exec()) {
        return;
    }

    while (query.next()) {
        int clubId = query.value(0).toInt();
        QString clubName = query.value(1).toString();
        ui->homeClubComboBox->addItem(clubName, clubId);
        ui->awayClubComboBox->addItem(clubName, clubId);
    }
}

void AdminMatchPage::loadTournaments()
{
    ui->tournamentComboBox->clear();

    QSqlQuery query;
    query.prepare("SELECT tournament_id, name FROM tournaments ORDER BY name"); // Используем prepare()

    if (!query.exec()) {
        return;
    }

    while (query.next()) {
        int tournamentId = query.value(0).toInt();
        QString tournamentName = query.value(1).toString();
        ui->tournamentComboBox->addItem(tournamentName, tournamentId);
    }
}

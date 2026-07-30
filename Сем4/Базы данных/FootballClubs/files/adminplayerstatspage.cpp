#include "adminplayerstatspage.h"
#include "ui_adminplayerstatspage.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QCompleter>

AdminPlayerStatsPage::AdminPlayerStatsPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::AdminPlayerStatsPage)
{
    ui->setupUi(this);

    proxyModel = new QSortFilterProxyModel(this);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);

    connect(ui->addButton, &QPushButton::clicked, this, &AdminPlayerStatsPage::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &AdminPlayerStatsPage::onEditButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AdminPlayerStatsPage::onDeleteButtonClicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &AdminPlayerStatsPage::onExportButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &AdminPlayerStatsPage::onSearchButtonClicked);
    connect(ui->tableView, &QTableView::clicked, this, &AdminPlayerStatsPage::onRowSelected);

    loadMatches();
    loadAllStats();
    setupPlayerCompleter();
}

AdminPlayerStatsPage::~AdminPlayerStatsPage()
{
    delete ui;
}

void AdminPlayerStatsPage::loadAllStats(const QString &filter)
{
    QString queryStr = R"(
        SELECT ps.player_match_id AS ID,
               p.first_name || ' ' || p.last_name AS Игрок,
               hc.name || ' vs ' || ac.name || ' (' || m.score || ')' AS Матч,
               ps.goals AS "Голы",
               ps.assists AS "Ассисты",
               ps.minutes_played AS "Минуты",
               ps.starting_lineup AS "В стартовом составе"
        FROM player_stats ps
        LEFT JOIN players p ON ps.player_id = p.player_id
        LEFT JOIN matches m ON ps.match_id = m.match_id
        LEFT JOIN clubs hc ON m.home_club_id = hc.club_id
        LEFT JOIN clubs ac ON m.away_club_id = ac.club_id
    )";

    if (!filter.isEmpty()) {
        queryStr += " WHERE p.first_name ILIKE '%" + filter + "%' OR p.last_name ILIKE '%" + filter + "%'";
    }

    queryStr += " ORDER BY ps.player_match_id";

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        return;
    }

    proxyModel->setSourceModel(model);
    proxyModel->sort(0, Qt::AscendingOrder);
    ui->tableView->resizeColumnsToContents();
}

void AdminPlayerStatsPage::onAddButtonClicked()
{
    QString playerName = ui->playerSearchEdit->text().trimmed();
    int playerId = -1;

    // Найти ID игрока по имени
    QSqlQuery query;
    query.prepare("SELECT player_id FROM players WHERE first_name || ' ' || last_name = :full_name");
    query.bindValue(":full_name", playerName);

    if (query.exec() && query.next()) {
        playerId = query.value(0).toInt();
    }

    if (playerId == -1) {
        QMessageBox::warning(this, "Ошибка", "Игрок с таким именем не найден!");
        return;
    }

    int matchId = ui->matchComboBox->currentData().toInt();
    int goals = ui->goalsEdit->text().toInt();
    int assists = ui->assistsEdit->text().toInt();
    int minutesPlayed = ui->minutesPlayedEdit->text().toInt();
    bool startingLineup = ui->startingLineupCheckBox->isChecked();

    if (matchId == 0) {
        QMessageBox::warning(this, "Ошибка", "Выберите матч!");
        return;
    }

    QSqlQuery idQuery;
    idQuery.prepare("SELECT COALESCE(MAX(player_match_id), 0) + 1 FROM player_stats");
    if (!idQuery.exec() || !idQuery.next()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить новый ID статистики!\n" + idQuery.lastError().text());
        return;
    }

    int newStatId = idQuery.value(0).toInt();

    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO player_stats (player_match_id, match_id, player_id, goals, assists, minutes_played, starting_lineup) "
                        "VALUES (:player_match_id, :match_id, :player_id, :goals, :assists, :minutes_played, :starting_lineup)");
    insertQuery.bindValue(":player_match_id", newStatId);
    insertQuery.bindValue(":match_id", matchId);
    insertQuery.bindValue(":player_id", playerId);
    insertQuery.bindValue(":goals", goals);
    insertQuery.bindValue(":assists", assists);
    insertQuery.bindValue(":minutes_played", minutesPlayed);
    insertQuery.bindValue(":starting_lineup", startingLineup);

    if (!insertQuery.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить статистику!\n" + insertQuery.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Статистика успешно добавлена!");
        loadAllStats();
    }
}

void AdminPlayerStatsPage::onEditButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите статистику для редактирования!");
        return;
    }

    int statId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();
    QString playerName = ui->playerSearchEdit->text().trimmed();
    int playerId = -1;

    // Найти ID игрока по имени
    QSqlQuery query;
    query.prepare("SELECT player_id FROM players WHERE first_name || ' ' || last_name = :full_name");
    query.bindValue(":full_name", playerName);

    if (query.exec() && query.next()) {
        playerId = query.value(0).toInt();
    }

    if (playerId == -1) {
        QMessageBox::warning(this, "Ошибка", "Игрок с таким именем не найден!");
        return;
    }

    int matchId = ui->matchComboBox->currentData().toInt();
    int goals = ui->goalsEdit->text().toInt();
    int assists = ui->assistsEdit->text().toInt();
    int minutesPlayed = ui->minutesPlayedEdit->text().toInt();
    bool startingLineup = ui->startingLineupCheckBox->isChecked();

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE player_stats SET player_id = :player_id, match_id = :match_id, goals = :goals, "
                        "assists = :assists, minutes_played = :minutes_played, starting_lineup = :starting_lineup "
                        "WHERE player_match_id = :player_match_id");
    updateQuery.bindValue(":player_id", playerId);
    updateQuery.bindValue(":match_id", matchId);
    updateQuery.bindValue(":goals", goals);
    updateQuery.bindValue(":assists", assists);
    updateQuery.bindValue(":minutes_played", minutesPlayed);
    updateQuery.bindValue(":starting_lineup", startingLineup);
    updateQuery.bindValue(":player_match_id", statId);

    if (!updateQuery.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные статистики!\n" + updateQuery.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Данные статистики успешно обновлены!");
        loadAllStats();
    }
}

void AdminPlayerStatsPage::onDeleteButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите статистику для удаления!");
        return;
    }

    int statId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM player_stats WHERE player_match_id = :id");
    query.bindValue(":id", statId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить статистику!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Статистика успешно удалена!");
        loadAllStats();
    }
}

void AdminPlayerStatsPage::onExportButtonClicked()
{
    exportToCSV(proxyModel, "Статистика игроков.csv");
}

void AdminPlayerStatsPage::onSearchButtonClicked()
{
    QString search = ui->searchEdit->text().trimmed();
    loadAllStats(search);
}

void AdminPlayerStatsPage::onRowSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    // Преобразуем индекс из proxyModel в исходный индекс модели
    QModelIndex sourceIndex = proxyModel->mapToSource(index);

    QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(proxyModel->sourceModel());
    if (!model) return;

    // Извлекаем данные из исходной модели
    QString playerName = model->data(model->index(sourceIndex.row(), 1)).toString();
    ui->playerSearchEdit->setText(playerName);

    QString matchInfo = model->data(model->index(sourceIndex.row(), 2)).toString();
    int matchIndex = ui->matchComboBox->findText(matchInfo);
    if (matchIndex != -1) {
        ui->matchComboBox->setCurrentIndex(matchIndex);
    }

    int goals = model->data(model->index(sourceIndex.row(), 3)).toInt();
    ui->goalsEdit->setText(QString::number(goals));

    int assists = model->data(model->index(sourceIndex.row(), 4)).toInt();
    ui->assistsEdit->setText(QString::number(assists));

    int minutesPlayed = model->data(model->index(sourceIndex.row(), 5)).toInt();
    ui->minutesPlayedEdit->setText(QString::number(minutesPlayed));

    bool startingLineup = model->data(model->index(sourceIndex.row(), 6)).toBool();
    ui->startingLineupCheckBox->setChecked(startingLineup);
}

void AdminPlayerStatsPage::loadMatches()
{
    ui->matchComboBox->clear();

    QSqlQuery query;
    query.prepare(R"(
        SELECT m.match_id, 
               hc.name || ' vs ' || ac.name || ' (' || m.score || ')' AS match_info
        FROM matches m
        LEFT JOIN clubs hc ON m.home_club_id = hc.club_id
        LEFT JOIN clubs ac ON m.away_club_id = ac.club_id
        ORDER BY m.match_date
    )");

    if (!query.exec()) {
        return;
    }

    while (query.next()) {
        int matchId = query.value(0).toInt();
        QString matchInfo = query.value(1).toString();
        ui->matchComboBox->addItem(matchInfo, matchId);
    }
}

void AdminPlayerStatsPage::setupPlayerCompleter()
{
    QSqlQuery query;
    query.prepare("SELECT first_name || ' ' || last_name AS full_name FROM players ORDER BY full_name");

    if (!query.exec()) {
        return;
    }

    QStringList playerNames;
    while (query.next()) {
        playerNames << query.value(0).toString();
    }

    QCompleter *completer = new QCompleter(playerNames, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->playerSearchEdit->setCompleter(completer); // `playerSearchEdit` — это поле для ввода имени игрока
}
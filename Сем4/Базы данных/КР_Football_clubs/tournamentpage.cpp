#include "tournamentpage.h"
#include "ui_tournamentpage.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDialog>
#include <QVBoxLayout>
#include <QTableView>
#include <QSortFilterProxyModel>

TournamentPage::TournamentPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TournamentPage)
    , currentTournamentId(1)
{
    ui->setupUi(this);
    loadTournament(currentTournamentId);
}

TournamentPage::~TournamentPage() { delete ui; }

void TournamentPage::loadTournament(int id)
{
    QSqlQuery query;
    query.prepare("SELECT name, year, country FROM tournaments WHERE tournament_id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next()) {
        return; // Tournament not found
    }
    currentTournamentId = id;
    ui->nameLabel->setText(query.value(0).toString());
    ui->yearLabel->setText(QString::number(query.value(1).toInt()));
    ui->locationLabel->setText(query.value(2).toString());
}

void TournamentPage::on_prevButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT tournament_id FROM tournaments WHERE tournament_id < :id ORDER BY tournament_id DESC LIMIT 1");
    query.bindValue(":id", currentTournamentId);
    if (query.exec() && query.next()) {
        loadTournament(query.value(0).toInt());
    }
}

void TournamentPage::on_nextButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT tournament_id FROM tournaments WHERE tournament_id > :id ORDER BY tournament_id ASC LIMIT 1");
    query.bindValue(":id", currentTournamentId);
    if (query.exec() && query.next()) {
        loadTournament(query.value(0).toInt());
    }
}

void TournamentPage::on_searchButton_clicked()
{
    QString namePrefix = ui->searchEdit->text().trimmed();
    if (namePrefix.isEmpty()) return;

    QSqlQuery query;
    query.prepare("SELECT tournament_id FROM tournaments WHERE name ILIKE :prefix");
    query.bindValue(":prefix", "%" + namePrefix + "%");
    if (query.exec() && query.next()) {
        loadTournament(query.value(0).toInt());
    }
}

void TournamentPage::on_clubsButton_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Турнирная таблица: " + ui->nameLabel->text());
    dlg.resize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QTableView *view = new QTableView;

    QString qStr = QString(
                       "SELECT c.name AS Клуб, "
                       "SUM(CASE WHEN (m.home_club_id = c.club_id AND SPLIT_PART(m.score, '-', 1)::INTEGER > SPLIT_PART(m.score, '-', 2)::INTEGER) OR "
                       "(m.away_club_id = c.club_id AND SPLIT_PART(m.score, '-', 2)::INTEGER > SPLIT_PART(m.score, '-', 1)::INTEGER) THEN 3 "
                       "WHEN SPLIT_PART(m.score, '-', 1)::INTEGER = SPLIT_PART(m.score, '-', 2)::INTEGER THEN 1 ELSE 0 END) AS Очки, "
                       "COUNT(CASE WHEN (m.home_club_id = c.club_id AND SPLIT_PART(m.score, '-', 1)::INTEGER > SPLIT_PART(m.score, '-', 2)::INTEGER) OR "
                       "(m.away_club_id = c.club_id AND SPLIT_PART(m.score, '-', 2)::INTEGER > SPLIT_PART(m.score, '-', 1)::INTEGER) THEN 1 ELSE NULL END) AS Победы, "
                       "COUNT(CASE WHEN SPLIT_PART(m.score, '-', 1)::INTEGER = SPLIT_PART(m.score, '-', 2)::INTEGER THEN 1 ELSE NULL END) AS Ничьи, "
                       "COUNT(CASE WHEN (m.home_club_id = c.club_id AND SPLIT_PART(m.score, '-', 1)::INTEGER < SPLIT_PART(m.score, '-', 2)::INTEGER) OR "
                       "(m.away_club_id = c.club_id AND SPLIT_PART(m.score, '-', 2)::INTEGER < SPLIT_PART(m.score, '-', 1)::INTEGER) THEN 1 ELSE NULL END) AS Поражения, "
                       "SUM(CASE WHEN m.home_club_id = c.club_id THEN SPLIT_PART(m.score, '-', 1)::INTEGER ELSE SPLIT_PART(m.score, '-', 2)::INTEGER END) AS Забито, "
                       "SUM(CASE WHEN m.home_club_id = c.club_id THEN SPLIT_PART(m.score, '-', 2)::INTEGER ELSE SPLIT_PART(m.score, '-', 1)::INTEGER END) AS Пропущено "
                       "FROM clubs c "
                       "LEFT JOIN matches m ON c.club_id = m.home_club_id OR c.club_id = m.away_club_id "
                       "WHERE m.tournament_id = %1 "
                       "GROUP BY c.club_id, c.name "
                       "ORDER BY Очки DESC, Забито DESC, Пропущено ASC"
                       ).arg(currentTournamentId);

    QSqlQueryModel *sqlModel = new QSqlQueryModel(&dlg);
    sqlModel->setQuery(qStr);

    // Используем QSortFilterProxyModel для сортировки
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(&dlg);
    proxyModel->setSourceModel(sqlModel);

    QPushButton *exportButton = new QPushButton("Экспорт в CSV", &dlg);
    connect(exportButton, &QPushButton::clicked, [&]() {
        exportToCSV(proxyModel, "Турнирная таблица " + ui->nameLabel->text() + ".csv");
    });
    layout->addWidget(exportButton);

    view->setModel(proxyModel);
    view->setSortingEnabled(true); // Включаем сортировку
    view->resizeColumnsToContents();
    layout->addWidget(view);
    dlg.exec();
}

void TournamentPage::on_matchesButton_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Матчи в " + ui->nameLabel->text());
    dlg.resize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QTableView *view = new QTableView;

    QString qStr = QString(
                       "SELECT m.match_date as \"Дата матча\", hc.name AS \"Клуб хозяин\", ac.name AS \"Клуб гость\", m.score as Счет "
                       "FROM matches m "
                       "JOIN clubs hc ON m.home_club_id = hc.club_id "
                       "JOIN clubs ac ON m.away_club_id = ac.club_id "
                       "WHERE m.tournament_id = %1"
                       ).arg(currentTournamentId);

    QSqlQueryModel *sqlModel = new QSqlQueryModel(&dlg);
    sqlModel->setQuery(qStr);

    // Используем QSortFilterProxyModel для сортировки
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(&dlg);
    proxyModel->setSourceModel(sqlModel);

    QPushButton *exportButton = new QPushButton("Экспорт в CSV", &dlg);
    connect(exportButton, &QPushButton::clicked, [&]() {
        exportToCSV(proxyModel, "Матчи " + ui->nameLabel->text() + ".csv");
    });
    layout->addWidget(exportButton);

    view->setModel(proxyModel);
    view->setSortingEnabled(true); // Включаем сортировку
    view->resizeColumnsToContents();
    layout->addWidget(view);
    dlg.exec();
}

void TournamentPage::on_topScorersButton_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Таблица бомбардиров: " + ui->nameLabel->text());
    dlg.resize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QTableView *view = new QTableView;

    QString qStr = QString(
                       "SELECT p.first_name || ' ' || p.last_name AS \"Имя\", "
                       "p.birth AS \"Дата рождения\", "
                       "p.nationality AS \"Национальность\", "
                       "p.position AS \"Позиция\", "
                       "c.name AS \"Клуб\", "
                       "COUNT(ps.match_id) AS \"Матчи\", "
                       "COALESCE(SUM(ps.goals), 0) AS \"Голы\", "
                       "COALESCE(SUM(ps.assists), 0) AS \"Передачи\" "
                       "FROM players p "
                       "JOIN clubs c ON p.club_id = c.club_id "
                       "JOIN matches m ON (m.home_club_id = c.club_id OR m.away_club_id = c.club_id) "
                       "LEFT JOIN player_stats ps ON ps.player_id = p.player_id AND ps.match_id = m.match_id "
                       "WHERE m.tournament_id = %1 "
                       "GROUP BY p.player_id, p.first_name, p.last_name, p.birth, p.nationality, p.position, c.name "
                       "ORDER BY \"Голы\" DESC, \"Передачи\" DESC"
                       ).arg(currentTournamentId);

    QSqlQueryModel *sqlModel = new QSqlQueryModel(&dlg);
    sqlModel->setQuery(qStr);

    // Используем QSortFilterProxyModel для сортировки
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(&dlg);
    proxyModel->setSourceModel(sqlModel);
    QPushButton *exportButton = new QPushButton("Экспорт в CSV", &dlg);
    connect(exportButton, &QPushButton::clicked, [&]() {
        exportToCSV(proxyModel, "Таблица бомбардиров " + ui->nameLabel->text() + ".csv");
    });
    layout->addWidget(exportButton);

    view->setModel(proxyModel);
    view->setSortingEnabled(true); // Включаем сортировку
    view->resizeColumnsToContents();
    layout->addWidget(view);
    dlg.exec();
}

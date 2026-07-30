#include "clubpage.h"
#include "ui_clubpage.h"
#include "playerpage.h"
#include "coachpage.h"
#include "utils.h"
#include <QSqlQuery>
#include <QPixmap>
#include <QDir>
#include <QSqlQueryModel>
#include <QDialog>
#include <QVBoxLayout>
#include <QTableView>


ClubPage::ClubPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClubPage)
    , currentClubId(1)
{
    ui->setupUi(this);
    loadClub(currentClubId);
}

ClubPage::~ClubPage() { delete ui; }

void ClubPage::loadClub(int id)
{
    QSqlQuery query;
    query.prepare("SELECT name, city, logo_path FROM clubs WHERE club_id = :id");
    query.bindValue(":id", id);
    if (!query.exec() || !query.next()) {
        return; // Club not found, stay on current
    }
    currentClubId = id;
    ui->nameLabel->setText(query.value(0).toString());
    ui->cityLabel->setText(query.value(1).toString());

    // Load and display logo
    QString logoPath = query.value(2).toString();
    if (logoPath.startsWith("~/")) {
        logoPath = QDir::homePath() + logoPath.mid(1);
    }
    QPixmap pix(logoPath);
    if (pix.isNull()) {
        ui->logoLabel->clear();
        qWarning() << "Failed to load pixmap from" << logoPath;
    } else {
        ui->logoLabel->setPixmap(pix.scaled(500,500, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void ClubPage::on_prevButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT club_id FROM clubs WHERE club_id < :id ORDER BY club_id DESC LIMIT 1");
    query.bindValue(":id", currentClubId);
    if (query.exec() && query.next()) {
        loadClub(query.value(0).toInt());
    }
}

void ClubPage::on_nextButton_clicked()
{
    QSqlQuery query;
    query.prepare("SELECT club_id FROM clubs WHERE club_id > :id ORDER BY club_id ASC LIMIT 1");
    query.bindValue(":id", currentClubId);
    if (query.exec() && query.next()) {
        loadClub(query.value(0).toInt());
    }
}

void ClubPage::on_searchButton_clicked()
{
    QString namePrefix = ui->searchEdit->text().trimmed();
    if (namePrefix.isEmpty()) return;

    QSqlQuery query;
    query.prepare("SELECT club_id FROM clubs WHERE name ILIKE :prefix");
    query.bindValue(":prefix", "%" + namePrefix + "%");
    if (query.exec() && query.next()) {
        loadClub(query.value(0).toInt());
    }
    // If not found, do nothing silently
}

void ClubPage::on_playersButton_clicked()
{
    // Players dialog
    QDialog dlg(this);
    dlg.setWindowTitle("Игроки " + ui->nameLabel->text());
    dlg.resize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QTableView *view = new QTableView;

    // Build query string with club filter including all player fields and stats
    QString qStr = QString(
                       "SELECT p.first_name || ' ' || p.last_name as Имя, p.birth as \"Дата рождения\", p.nationality as Национальность, p.position as Позиция, "
                       "COUNT(ps.match_id) AS Матчи, COALESCE(SUM(ps.goals),0) AS Голы, COALESCE(SUM(ps.assists),0) AS Передачи "
                       "FROM players p "
                       "LEFT JOIN player_stats ps ON p.player_id = ps.player_id "
                       "WHERE p.club_id = %1 "
                       "GROUP BY p.player_id, p.first_name, p.last_name, p.birth, p.nationality, p.position"
                       ).arg(currentClubId);

    QSqlQueryModel *sqlModel = new QSqlQueryModel(&dlg);
    sqlModel->setQuery(qStr);

    // Используем QSortFilterProxyModel для сортировки
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(&dlg);
    proxyModel->setSourceModel(sqlModel);

    view->setModel(proxyModel);
    view->setSortingEnabled(true); // Включаем сортировку
    view->resizeColumnsToContents();

    QPushButton *exportButton = new QPushButton("Экспорт в CSV", &dlg);
    connect(exportButton, &QPushButton::clicked, [&]() {
        exportToCSV(proxyModel, "Игроки " + ui->nameLabel->text() + ".csv");
    });
    layout->addWidget(exportButton);

    layout->addWidget(view);
    dlg.exec();
}

void ClubPage::on_coachesButton_clicked()
{
    // Coaches dialog
    QDialog dlg(this);
    dlg.setWindowTitle("Тренеры " + ui->nameLabel->text());
    dlg.resize(600, 400);
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QTableView *view = new QTableView;

    QString qStr = QString(
                       "SELECT first_name || ' ' || last_name AS Имя, birth as \"Дата рождения\", job as Должность "
                       "FROM coaches WHERE club_id = %1"
                       ).arg(currentClubId);

    QSqlQueryModel *sqlModel = new QSqlQueryModel(&dlg);
    sqlModel->setQuery(qStr);

    // Используем QSortFilterProxyModel для сортировки
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(&dlg);
    proxyModel->setSourceModel(sqlModel);

    view->setModel(proxyModel);
    view->setSortingEnabled(true); // Включаем сортировку
    view->resizeColumnsToContents();

    QPushButton *exportButton = new QPushButton("Экспорт в CSV", &dlg);
    connect(exportButton, &QPushButton::clicked, [&]() {
        exportToCSV(proxyModel, "Тренеры " + ui->nameLabel->text() + ".csv");
    });
    layout->addWidget(exportButton);
    layout->addWidget(view);
    dlg.exec();
}

void ClubPage::on_matchesButton_clicked()
{
    // Matches dialog
    QDialog dlg(this);
    dlg.setWindowTitle("Матчи клуба: " + ui->nameLabel->text());
    dlg.resize(800, 600);
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QTableView *view = new QTableView;

    QString qStr = QString(
                       "SELECT m.match_date AS \"Дата матча\", "
                       "hc.name AS \"Клуб хозяин\", "
                       "m.score AS \"Счет\", "
                       "ac.name AS \"Клуб гость\", "
                       "t.name AS \"Турнир\" "
                       "FROM matches m "
                       "JOIN clubs hc ON m.home_club_id = hc.club_id "
                       "JOIN clubs ac ON m.away_club_id = ac.club_id "
                       "JOIN tournaments t ON m.tournament_id = t.tournament_id "
                       "WHERE m.home_club_id = %1 OR m.away_club_id = %1 "
                       "ORDER BY m.match_date DESC"
                       ).arg(currentClubId);

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

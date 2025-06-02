#include "playerpage.h"
#include "ui_playerpage.h"
#include "utils.h"
#include <QSqlQueryModel>
#include <QSqlQuery>

PlayerPage::PlayerPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlayerPage)
{
    ui->setupUi(this);
    proxyModel = new QSortFilterProxyModel(this);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true); // Включаем сортировку

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, [=](int logicalIndex) {
        Qt::SortOrder newOrder = Qt::AscendingOrder;
        if (lastSortColumn == logicalIndex && lastSortOrder == Qt::AscendingOrder) {
            newOrder = Qt::DescendingOrder;
        }
        lastSortColumn = logicalIndex;
        lastSortOrder = newOrder;
        proxyModel->sort(logicalIndex, newOrder);
    });
    //connect(ui->exportButton, &QPushButton::clicked, this, &PlayerPage::on_exportButton_clicked);
    loadAllPlayers();
}

PlayerPage::~PlayerPage() {
    delete ui;
}

void PlayerPage::loadAllPlayers(const QString &filter) {
    QString queryStr = R"(
        SELECT p.first_name || ' ' || p.last_name AS Имя,
               p.birth as "Дата рождения", p.nationality as Национальность, p.position as Позиция,
               c.name AS Клуб,
               COUNT(ps.match_id) AS Матчи,
               COALESCE(SUM(ps.goals),0) AS Голы,
               COALESCE(SUM(ps.assists),0) AS Передачи
        FROM players p
        LEFT JOIN player_stats ps ON p.player_id = ps.player_id
        LEFT JOIN clubs c ON p.club_id = c.club_id
    )";

    if (!filter.isEmpty()) {
        queryStr += " WHERE p.first_name ILIKE :filter OR p.last_name ILIKE :filter ";
    }

    queryStr += " GROUP BY p.player_id, c.name ORDER BY p.player_id";

    QSqlQuery query;
    query.prepare(queryStr);

    if (!filter.isEmpty()) {
        query.bindValue(":filter", "%" + filter + "%");
    }

    query.exec();

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);
    proxyModel->setSourceModel(model);
    ui->tableView->resizeColumnsToContents();
}



void PlayerPage::on_searchButton_clicked() {
    QString search = ui->searchEdit->text().trimmed();
    loadAllPlayers(search);
}


void PlayerPage::on_exportButton_clicked()
{
    exportToCSV(proxyModel, "Игроки.csv");
}

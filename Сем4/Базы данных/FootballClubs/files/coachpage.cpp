#include "coachpage.h"
#include "ui_coachpage.h"
#include "utils.h"
#include <QSqlQueryModel>
#include <QSqlQuery>

CoachPage::CoachPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::CoachPage)
{
    ui->setupUi(this);
    proxyModel = new QSortFilterProxyModel(this);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);

    connect(ui->tableView->horizontalHeader(), &QHeaderView::sectionClicked, this, [=](int logicalIndex) {
        Qt::SortOrder newOrder = Qt::AscendingOrder;
        if (lastSortColumn == logicalIndex && lastSortOrder == Qt::AscendingOrder) {
            newOrder = Qt::DescendingOrder;
        }
        lastSortColumn = logicalIndex;
        lastSortOrder = newOrder;
        proxyModel->sort(logicalIndex, newOrder);
    });
    loadAllCoaches();
}

CoachPage::~CoachPage() {
    delete ui;
}

void CoachPage::loadAllCoaches(const QString &filter) {
    QString queryStr = R"(
        SELECT first_name || ' ' || last_name AS Имя,
               birth as "Дата рождения", job as Должность,
               c.name AS Клуб
        FROM coaches
        LEFT JOIN clubs c ON coaches.club_id = c.club_id
    )";

    if (!filter.isEmpty()) {
        queryStr += " WHERE first_name ILIKE :filter OR last_name ILIKE :filter ";
    }

    queryStr += " ORDER BY coach_id";

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
    //ui->clubLabel->setText("All Coaches");
}



void CoachPage::on_searchButton_clicked() {
    QString search = ui->searchEdit->text().trimmed();
    loadAllCoaches(search);
}

void CoachPage::on_exportButton_clicked()
{
    exportToCSV(proxyModel, "Тренеры.csv");
}

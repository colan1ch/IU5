#include "admincoachpage.h"
#include "ui_admincoachpage.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

AdminCoachPage::AdminCoachPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::AdminCoachPage)
{
    ui->setupUi(this);

    proxyModel = new QSortFilterProxyModel(this);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);

    connect(ui->addButton, &QPushButton::clicked, this, &AdminCoachPage::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &AdminCoachPage::onEditButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AdminCoachPage::onDeleteButtonClicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &AdminCoachPage::onExportButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &AdminCoachPage::onSearchButtonClicked);
    connect(ui->tableView, &QTableView::clicked, this, &AdminCoachPage::onRowSelected);
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &AdminCoachPage::onSortChanged);

    loadClubs();
    loadAllCoaches();
}

AdminCoachPage::~AdminCoachPage()
{
    delete ui;
}

void AdminCoachPage::loadAllCoaches(const QString &filter)
{
    QString queryStr = R"(
        SELECT coach_id AS ID,
               first_name || ' ' || last_name AS Имя,
               birth AS "Дата рождения",
               job AS "Должность",
               c.name AS Клуб
        FROM coaches
        LEFT JOIN clubs c ON coaches.club_id = c.club_id
    )";

    if (!filter.isEmpty()) {
        queryStr += " WHERE first_name ILIKE :filter OR last_name ILIKE :filter ";
    }

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

void AdminCoachPage::onAddButtonClicked()
{
    QString fullName = ui->nameEdit->text().trimmed();
    QStringList nameParts = fullName.split(" ", Qt::SkipEmptyParts);

    QString firstName = nameParts.value(0);
    QString lastName = nameParts.size() > 1 ? nameParts.value(1) : "";

    QString birth = ui->birthDateEdit->date().toString("yyyy-MM-dd");
    QString job = ui->jobComboBox->currentText().trimmed();
    int clubId = ui->clubComboBox->currentData().toInt();

    if (firstName.isEmpty() || lastName.isEmpty() || birth.isEmpty() || job.isEmpty() || clubId == 0) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO coaches (first_name, last_name, birth, job, club_id) "
                  "VALUES (:first_name, :last_name, :birth, :job, :club_id)");
    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":birth", birth);
    query.bindValue(":job", job);
    query.bindValue(":club_id", clubId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить тренера!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Тренер успешно добавлен!");
        loadAllCoaches();
    }
}

void AdminCoachPage::onEditButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите тренера для редактирования!");
        return;
    }

    int coachId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();

    QString fullName = ui->nameEdit->text().trimmed();
    QStringList nameParts = fullName.split(" ", Qt::SkipEmptyParts);

    QString firstName = nameParts.value(0);
    QString lastName = nameParts.size() > 1 ? nameParts.value(1) : "";

    QString birth = ui->birthDateEdit->date().toString("yyyy-MM-dd");
    QString job = ui->jobComboBox->currentText().trimmed();
    int clubId = ui->clubComboBox->currentData().toInt();

    if (firstName.isEmpty() || lastName.isEmpty() || birth.isEmpty() || job.isEmpty() || clubId == 0) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE coaches SET first_name = :first_name, last_name = :last_name, birth = :birth, "
                  "job = :job, club_id = :club_id WHERE coach_id = :coach_id");
    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":birth", birth);
    query.bindValue(":job", job);
    query.bindValue(":club_id", clubId);
    query.bindValue(":coach_id", coachId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные тренера!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Данные тренера успешно обновлены!");
        loadAllCoaches();
    }
}

void AdminCoachPage::onDeleteButtonClicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите тренера для удаления!");
        return;
    }

    int coachId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM coaches WHERE coach_id = :id");
    query.bindValue(":id", coachId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить тренера!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Тренер успешно удален!");
        loadAllCoaches();
    }
}

void AdminCoachPage::onExportButtonClicked()
{
    exportToCSV(proxyModel, "Тренеры.csv");
}

void AdminCoachPage::onSearchButtonClicked()
{
    QString search = ui->searchEdit->text().trimmed();
    loadAllCoaches(search);
}

void AdminCoachPage::onRowSelected(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }

    // Получаем модель данных таблицы
    QAbstractItemModel *model = proxyModel; // Используем proxyModel, так как он подключен к tableView
    if (!model) {
        return;
    }

    int row = index.row();

    // Извлечение данных из модели
    QString fullName = model->data(model->index(row, 1)).toString(); // Полное имя
    QString birth = model->data(model->index(row, 2)).toString();    // Дата рождения
    QString job = model->data(model->index(row, 3)).toString();      // Должность
    QString club = model->data(model->index(row, 4)).toString();     // Клуб

    // Разделяем полное имя на имя и фамилию
    QStringList nameParts = fullName.split(" ", Qt::SkipEmptyParts);
    QString firstName = nameParts.value(0);
    QString lastName = nameParts.size() > 1 ? nameParts.value(1) : "";

    // Установка данных в поля ввода
    ui->nameEdit->setText(firstName + " " + lastName); // Полное имя
    ui->birthDateEdit->setDate(QDate::fromString(birth, "yyyy-MM-dd")); // Дата рождения

    int jobIndex = ui->jobComboBox->findText(job);
    if (jobIndex != -1) {
        ui->jobComboBox->setCurrentIndex(jobIndex); // Установка должности
    }

    int clubIndex = ui->clubComboBox->findText(club);
    if (clubIndex != -1) {
        ui->clubComboBox->setCurrentIndex(clubIndex); // Установка клуба
    }
}

void AdminCoachPage::onSortChanged(int logicalIndex, Qt::SortOrder order)
{
    QString sortColumn;

    switch (logicalIndex) {
        case 0: sortColumn = "coach_id"; break;
        case 1: sortColumn = "first_name || ' ' || last_name"; break;
        case 2: sortColumn = "birth"; break;
        case 3: sortColumn = "job"; break;
        case 4: sortColumn = "c.name"; break;
        default: sortColumn = "coach_id"; break;
    }

    QString queryStr = QString(R"(
        SELECT coach_id AS ID,
               first_name || ' ' || last_name AS Имя,
               birth AS "Дата рождения",
               job AS "Должность",
               c.name AS Клуб
        FROM coaches
        LEFT JOIN clubs c ON coaches.club_id = c.club_id
        ORDER BY %1 %2
    )").arg(sortColumn, order == Qt::AscendingOrder ? "ASC" : "DESC");

    QSqlQuery query;
    query.prepare(queryStr);
    query.exec();

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);
    proxyModel->setSourceModel(model);
    ui->tableView->resizeColumnsToContents();
}

void AdminCoachPage::loadClubs()
{
    ui->clubComboBox->clear();

    QSqlQuery query;
    query.prepare("SELECT club_id, name FROM clubs ORDER BY name");

    if (!query.exec()) {
        return;
    }

    while (query.next()) {
        int clubId = query.value(0).toInt();
        QString clubName = query.value(1).toString();
        ui->clubComboBox->addItem(clubName, clubId);
    }

}

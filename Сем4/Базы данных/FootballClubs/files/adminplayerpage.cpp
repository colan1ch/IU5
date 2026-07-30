#include "adminplayerpage.h"
#include "ui_adminplayerpage.h"
#include "utils.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>

AdminPlayerPage::AdminPlayerPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::AdminPlayerPage)
{
    ui->setupUi(this);

    proxyModel = new QSortFilterProxyModel(this);
    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);

    connect(ui->addButton, &QPushButton::clicked, this, &AdminPlayerPage::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &AdminPlayerPage::onEditButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AdminPlayerPage::onDeleteButtonClicked);
    connect(ui->exportButton, &QPushButton::clicked, this, &AdminPlayerPage::onExportButtonClicked);
    connect(ui->searchButton, &QPushButton::clicked, this, &AdminPlayerPage::onSearchButtonClicked);
    connect(ui->tableView, &QTableView::clicked, this, &AdminPlayerPage::onRowSelected);
    connect(ui->tableView->horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &AdminPlayerPage::onSortChanged);

    loadClubs();
    loadAllPlayers();
}

AdminPlayerPage::~AdminPlayerPage()
{
    delete ui;
}

void AdminPlayerPage::loadAllPlayers(const QString &filter)
{
    QString queryStr = R"(
        SELECT player_id AS ID,
               first_name || ' ' || last_name AS Имя,
               birth AS "Дата рождения",
               nationality AS Национальность,
               position AS Позиция,
               c.name AS Клуб
        FROM players
        LEFT JOIN clubs c ON players.club_id = c.club_id
    )";

    if (!filter.isEmpty()) {
        queryStr += " WHERE first_name ILIKE :filter OR last_name ILIKE :filter ";
    }

    QSqlQuery query;
    query.prepare(queryStr);

    if (!filter.isEmpty()) {
        query.bindValue(":filter", "%" + filter + "%");
    }

    query.exec();

    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);
    proxyModel->setSourceModel(model);
    proxyModel->sort(0, Qt::AscendingOrder);
    ui->tableView->resizeColumnsToContents();
}

void AdminPlayerPage::onAddButtonClicked()
{
    QString fullName = ui->nameEdit->text().trimmed(); // Получаем полное имя из одного поля
    QStringList nameParts = fullName.split(" ", Qt::SkipEmptyParts); // Разделяем по пробелу

    QString firstName = nameParts.value(0); // Первое слово — это first_name
    QString lastName = nameParts.size() > 1 ? nameParts.value(1) : ""; // Второе слово (если есть) — это last_name

    QString birth = ui->birthEdit->text().trimmed();
    QString nationality = ui->nationEdit->text().trimmed();
    QString position = ui->positionComboBox->currentText().trimmed();
    int clubId = ui->clubComboBox->currentData().toInt();

    if (firstName.isEmpty() || birth.isEmpty() || nationality.isEmpty() || position.isEmpty() || clubId == 0) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    // Получаем максимальный player_id из таблицы
    QSqlQuery idQuery;
    idQuery.prepare("SELECT COALESCE(MAX(player_id), 0) + 1 FROM players");
    if (!idQuery.exec() || !idQuery.next()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить новый ID игрока!\n" + idQuery.lastError().text());
        return;
    }

    int newPlayerId = idQuery.value(0).toInt(); // Новый player_id

    // Выполняем запрос на добавление игрока
    QSqlQuery query;
    query.prepare("INSERT INTO players (player_id, first_name, last_name, birth, nationality, position, club_id) "
                  "VALUES (:player_id, :first_name, :last_name, :birth, :nationality, :position, :club_id)");
    query.bindValue(":player_id", newPlayerId);
    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":birth", birth);
    query.bindValue(":nationality", nationality);
    query.bindValue(":position", position);
    query.bindValue(":club_id", clubId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось добавить игрока!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Игрок успешно добавлен!");
        loadAllPlayers();
    }
}

void AdminPlayerPage::onEditButtonClicked()
{
    QString fullName = ui->nameEdit->text().trimmed();
    QStringList nameParts = fullName.split(" ", Qt::SkipEmptyParts);

    QString firstName = nameParts.value(0);
    QString lastName = nameParts.size() > 1 ? nameParts.value(1) : "";

    QString birth = ui->birthEdit->text().trimmed();
    QString nationality = ui->nationEdit->text().trimmed();
    QString position = ui->positionComboBox->currentText().trimmed();
    int clubId = ui->clubComboBox->currentData().toInt();

    if (firstName.isEmpty() || birth.isEmpty() || nationality.isEmpty() || position.isEmpty() || clubId == 0) {
        QMessageBox::warning(this, "Ошибка", "Все поля должны быть заполнены!");
        return;
    }

    // Получаем player_id из выбранной строки таблицы
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите игрока для редактирования!");
        return;
    }

    int playerId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();

    QSqlQuery query;
    query.prepare("UPDATE players SET first_name = :first_name, last_name = :last_name, birth = :birth, "
                  "nationality = :nationality, position = :position, club_id = :club_id WHERE player_id = :player_id");
    query.bindValue(":first_name", firstName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":birth", birth);
    query.bindValue(":nationality", nationality);
    query.bindValue(":position", position);
    query.bindValue(":club_id", clubId);
    query.bindValue(":player_id", playerId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось обновить данные игрока!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Данные игрока успешно обновлены!");
        loadAllPlayers();
    }
}

void AdminPlayerPage::onDeleteButtonClicked()
{
    // Получаем player_id из выбранной строки таблицы
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Ошибка", "Выберите игрока для удаления!");
        return;
    }

    int playerId = proxyModel->data(proxyModel->index(currentIndex.row(), 0)).toInt();

    // Выполняем запрос на удаление игрока
    QSqlQuery query;
    query.prepare("DELETE FROM players WHERE player_id = :id");
    query.bindValue(":id", playerId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось удалить игрока!\n" + query.lastError().text());
    } else {
        QMessageBox::information(this, "Успех", "Игрок успешно удален!");
        loadAllPlayers();
    }
}

void AdminPlayerPage::onExportButtonClicked()
{
    exportToCSV(proxyModel, "Игроки.csv"); // Используем вашу функцию exportToCSV
}

void AdminPlayerPage::onSearchButtonClicked()
{
    QString search = ui->searchEdit->text().trimmed();
    loadAllPlayers(search);
}

void AdminPlayerPage::loadClubs()
{
    ui->clubComboBox->clear(); // Очищаем список перед загрузкой

    QSqlQuery query("SELECT club_id, name FROM clubs ORDER BY name");
    while (query.next()) {
        int clubId = query.value(0).toInt();
        QString clubName = query.value(1).toString();
        ui->clubComboBox->addItem(clubName, clubId); // Добавляем название клуба и его ID в QComboBox
    }
}

void AdminPlayerPage::onRowSelected(const QModelIndex &index)
{
    if (!index.isValid()) return;

    // Преобразуем индекс из proxyModel в исходный индекс модели
    QModelIndex sourceIndex = proxyModel->mapToSource(index);

    QSqlQueryModel *model = qobject_cast<QSqlQueryModel *>(proxyModel->sourceModel());
    if (!model) return;

    // Извлекаем данные из исходной модели
    QString fullName = model->data(model->index(sourceIndex.row(), 1)).toString(); // Имя (first_name + last_name)
    ui->nameEdit->setText(fullName);

    QString birth = model->data(model->index(sourceIndex.row(), 2)).toString(); // Дата рождения
    ui->birthEdit->setDate(QDate::fromString(birth, "yyyy-MM-dd")); // Устанавливаем дату в QDateEdit

    QString nation = model->data(model->index(sourceIndex.row(), 3)).toString(); // Национальность
    ui->nationEdit->setText(nation);

    QString position = model->data(model->index(sourceIndex.row(), 4)).toString(); // Позиция
    int positionIndex = ui->positionComboBox->findText(position);
    if (positionIndex != -1) {
        ui->positionComboBox->setCurrentIndex(positionIndex);
    }

    // Устанавливаем выбранный клуб в QComboBox
    QString clubName = model->data(model->index(sourceIndex.row(), 5)).toString(); // Название клуба
    int clubIndex = ui->clubComboBox->findText(clubName);
    if (clubIndex != -1) {
        ui->clubComboBox->setCurrentIndex(clubIndex);
    }
}

void AdminPlayerPage::onSortChanged(int logicalIndex, Qt::SortOrder order)
{
    QString sortColumn;

    // Определяем, по какому столбцу сортировать
    switch (logicalIndex) {
        case 0: sortColumn = "player_id"; break; // ID
        case 1: sortColumn = "first_name || ' ' || last_name"; break; // Имя
        case 2: sortColumn = "birth"; break; // Дата рождения
        case 3: sortColumn = "nationality"; break; // Национальность
        case 4: sortColumn = "position"; break; // Позиция
        case 5: sortColumn = "c.name"; break; // Клуб
        default: sortColumn = "player_id"; break; // По умолчанию сортируем по ID
    }

    // Формируем запрос с учетом сортировки
    QString queryStr = QString(R"(
        SELECT player_id AS ID,
               first_name || ' ' || last_name AS Имя,
               birth AS "Дата рождения",
               nationality AS Национальность,
               position AS Позиция,
               c.name AS Клуб
        FROM players
        LEFT JOIN clubs c ON players.club_id = c.club_id
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
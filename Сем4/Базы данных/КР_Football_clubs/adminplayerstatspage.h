#ifndef ADMINPLAYERSTATSPAGE_H
#define ADMINPLAYERSTATSPAGE_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QModelIndex>

namespace Ui {
class AdminPlayerStatsPage;
}

class AdminPlayerStatsPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPlayerStatsPage(QWidget *parent = nullptr);
    ~AdminPlayerStatsPage();

private slots:
    void loadAllStats(const QString &filter = ""); // Загрузка всей статистики
    void onAddButtonClicked();                    // Добавление статистики
    void onEditButtonClicked();                   // Редактирование статистики
    void onDeleteButtonClicked();                 // Удаление статистики
    void onExportButtonClicked();                 // Экспорт данных в CSV
    void onSearchButtonClicked();                 // Поиск статистики
    void onRowSelected(const QModelIndex &index); // Обработка выбора строки

private:
    Ui::AdminPlayerStatsPage *ui;
    QSortFilterProxyModel *proxyModel;

    void loadPlayers(); // Загрузка списка игроков
    void loadMatches(); // Загрузка списка матчей
    void setupPlayerCompleter(); // Настройка автозаполнения для поля ввода имени игрока
};

#endif // ADMINPLAYERSTATSPAGE_H
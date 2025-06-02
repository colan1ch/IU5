#ifndef ADMINMATCHPAGE_H
#define ADMINMATCHPAGE_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QModelIndex>

namespace Ui {
class AdminMatchPage;
}

class AdminMatchPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMatchPage(QWidget *parent = nullptr);
    ~AdminMatchPage();

private slots:
    void loadAllMatches(const QString &filter = ""); // Загрузка всех матчей
    void onAddButtonClicked();                      // Добавление матча
    void onEditButtonClicked();                     // Редактирование матча
    void onDeleteButtonClicked();                   // Удаление матча
    void onExportButtonClicked();                   // Экспорт данных в CSV
    void onSearchButtonClicked();                   // Поиск матчей
    void onRowSelected(const QModelIndex &index);   // Обработка выбора строки

private:
    Ui::AdminMatchPage *ui;
    QSortFilterProxyModel *proxyModel;

    void loadClubs();       // Загрузка списка клубов
    void loadTournaments(); // Загрузка списка турниров
};

#endif // ADMINMATCHPAGE_H
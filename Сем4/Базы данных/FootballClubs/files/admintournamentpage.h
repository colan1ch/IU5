#ifndef ADMINTOURNAMENTPAGE_H
#define ADMINTOURNAMENTPAGE_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QModelIndex>

namespace Ui {
class AdminTournamentPage;
}

class AdminTournamentPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminTournamentPage(QWidget *parent = nullptr);
    ~AdminTournamentPage();

private slots:
    void loadAllTournaments(const QString &filter = ""); // Загрузка всех турниров
    void onAddButtonClicked();                          // Добавление турнира
    void onEditButtonClicked();                         // Редактирование турнира
    void onDeleteButtonClicked();                       // Удаление турнира
    void onExportButtonClicked();                       // Экспорт данных в CSV
    void onSearchButtonClicked();                       // Поиск турниров
    void onRowSelected(const QModelIndex &index);       // Обработка выбора строки

private:
    Ui::AdminTournamentPage *ui;
    QSortFilterProxyModel *proxyModel;
};

#endif // ADMINTOURNAMENTPAGE_H
#ifndef ADMINCLUBPAGE_H
#define ADMINCLUBPAGE_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QModelIndex>

namespace Ui {
class AdminClubPage;
}

class AdminClubPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminClubPage(QWidget *parent = nullptr);
    ~AdminClubPage();

private slots:
    void loadAllClubs(const QString &filter = ""); // Загрузка всех клубов
    void onAddButtonClicked();                    // Добавление клуба
    void onEditButtonClicked();                   // Редактирование клуба
    void onDeleteButtonClicked();                 // Удаление клуба
    void onExportButtonClicked();                 // Экспорт данных в CSV
    void onSearchButtonClicked();                 // Поиск клубов
    void onRowSelected(const QModelIndex &index); // Обработка выбора строки

private:
    Ui::AdminClubPage *ui;
    QSortFilterProxyModel *proxyModel;
};

#endif // ADMINCLUBPAGE_H
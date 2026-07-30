#ifndef ADMINPLAYERPAGE_H
#define ADMINPLAYERPAGE_H

#include <QWidget>
#include <QSortFilterProxyModel>

namespace Ui {
class AdminPlayerPage;
}

class AdminPlayerPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPlayerPage(QWidget *parent = nullptr);
    ~AdminPlayerPage();

private slots:
    void loadAllPlayers(const QString &filter = "");
    void onAddButtonClicked();
    void onEditButtonClicked();
    void onDeleteButtonClicked();
    void onExportButtonClicked();
    void onSearchButtonClicked();
    void onRowSelected(const QModelIndex &index); // Для обработки выбора строки
    void onSortChanged(int logicalIndex, Qt::SortOrder order); // Обработка сортировки

private:
    Ui::AdminPlayerPage *ui;
    QSortFilterProxyModel *proxyModel;

    void loadClubs(); // Метод для загрузки списка клубов в QComboBox
};

#endif // ADMINPLAYERPAGE_H
#ifndef ADMINCOACHPAGE_H
#define ADMINCOACHPAGE_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QDate>

namespace Ui {
class AdminCoachPage;
}

class AdminCoachPage : public QWidget
{
    Q_OBJECT

public:
    explicit AdminCoachPage(QWidget *parent = nullptr);
    ~AdminCoachPage();

private slots:
    void loadAllCoaches(const QString &filter = "");
    void onAddButtonClicked();
    void onEditButtonClicked();
    void onDeleteButtonClicked();
    void onExportButtonClicked();
    void onSearchButtonClicked();
    void onRowSelected(const QModelIndex &index);
    void onSortChanged(int logicalIndex, Qt::SortOrder order);

private:
    Ui::AdminCoachPage *ui;
    QSortFilterProxyModel *proxyModel;

    void loadClubs();
};

#endif // ADMINCOACHPAGE_H
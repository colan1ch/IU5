#ifndef PLAYERPAGE_H
#define PLAYERPAGE_H

#include <QWidget>
#include <QSortFilterProxyModel>


namespace Ui { class PlayerPage; }

class PlayerPage : public QWidget {
    Q_OBJECT
public:
    explicit PlayerPage(QWidget *parent = nullptr);
    ~PlayerPage();

private slots:
    void on_searchButton_clicked();
    void on_exportButton_clicked();

private:
    Ui::PlayerPage *ui;
    void loadAllPlayers(const QString &filter = "");
    QSortFilterProxyModel *proxyModel;
    int lastSortColumn = -1;
    Qt::SortOrder lastSortOrder = Qt::AscendingOrder;
};

#endif // PLAYERPAGE_H

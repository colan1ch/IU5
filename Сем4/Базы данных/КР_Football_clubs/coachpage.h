#ifndef COACHPAGE_H
#define COACHPAGE_H

#include <QWidget>
#include <QSortFilterProxyModel>


namespace Ui { class CoachPage; }

class CoachPage : public QWidget {
    Q_OBJECT
public:
    explicit CoachPage(QWidget *parent = nullptr);
    ~CoachPage();

private slots:
    void on_searchButton_clicked();
    void on_exportButton_clicked();

private:
    Ui::CoachPage *ui;
    void loadAllCoaches(const QString &filter = "");
    QSortFilterProxyModel *proxyModel;
    int lastSortColumn = -1;
    Qt::SortOrder lastSortOrder = Qt::AscendingOrder;

};

#endif // COACHPAGE_H

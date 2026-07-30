// File: clubpage.h
#ifndef CLUBPAGE_H
#define CLUBPAGE_H

#include <QWidget>
#include <QtSql>

namespace Ui { class ClubPage; }

class ClubPage : public QWidget {
    Q_OBJECT
public:
    explicit ClubPage(QWidget *parent = nullptr);
    ~ClubPage();

private slots:
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_searchButton_clicked();
    void on_playersButton_clicked();
    void on_coachesButton_clicked();
    void on_matchesButton_clicked();

private:
    Ui::ClubPage *ui;
    int currentClubId;
    void loadClub(int id);
};

#endif // CLUBPAGE_H

#ifndef TOURNAMENTPAGE_H
#define TOURNAMENTPAGE_H

#include <QWidget>

namespace Ui {
class TournamentPage;
}

class TournamentPage : public QWidget
{
    Q_OBJECT

public:
    explicit TournamentPage(QWidget *parent = nullptr);
    ~TournamentPage();

private slots:
    void loadTournament(int id);
    void on_prevButton_clicked();
    void on_nextButton_clicked();
    void on_searchButton_clicked();
    void on_clubsButton_clicked();
    void on_matchesButton_clicked();
    void on_topScorersButton_clicked();

private:
    Ui::TournamentPage *ui;
    int currentTournamentId;
};

#endif // TOURNAMENTPAGE_H

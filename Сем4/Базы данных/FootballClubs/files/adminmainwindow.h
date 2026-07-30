#ifndef ADMINMAINWINDOW_H
#define ADMINMAINWINDOW_H

#include <QMainWindow>

class AdminClubPage;
class AdminPlayerPage;
class AdminCoachPage;
class AdminTournamentPage;
class AdminMatchPage;
class AdminPlayerStatsPage;
//class LoginPage;

QT_BEGIN_NAMESPACE
namespace Ui { class AdminMainWindow; }
QT_END_NAMESPACE

class AdminMainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit AdminMainWindow(QWidget *parent = nullptr);
    ~AdminMainWindow();

private:
    Ui::AdminMainWindow *ui;

    // Страницы администратора
    AdminClubPage *adminClubPage;
    AdminPlayerPage *adminPlayerPage;
    AdminCoachPage *adminCoachPage;
    AdminTournamentPage *adminTournamentPage;
    AdminMatchPage *adminMatchPage;
    //LoginPage *loginPage; // Добавляем страницу входа
    AdminPlayerStatsPage *adminPlayerStatsPage; // Добавляем страницу статистики игроков
};

#endif // ADMINMAINWINDOW_H
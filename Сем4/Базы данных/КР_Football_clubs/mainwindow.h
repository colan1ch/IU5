#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class ClubPage;
class PlayerPage;
class CoachPage;
class TournamentPage; // Добавляем класс TournamentPage

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // Страницы
    ClubPage *clubPage;
    PlayerPage *playerPage;
    CoachPage *coachPage;
    TournamentPage *tournamentPage; // Добавляем указатель на TournamentPage
};

#endif // MAINWINDOW_H

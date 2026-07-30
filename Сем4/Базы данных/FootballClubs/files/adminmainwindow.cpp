#include "adminmainwindow.h"
#include "ui_adminmainwindow.h"

#include "adminclubpage.h"
#include "adminplayerpage.h"
#include "admincoachpage.h"
#include "admintournamentpage.h"
#include "adminmatchpage.h"
#include "adminplayerstatspage.h"
#include "loginpage.h"

AdminMainWindow::AdminMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminMainWindow)
{
    ui->setupUi(this);

    // Создание страниц администратора
    adminClubPage = new AdminClubPage(this);
    adminPlayerPage = new AdminPlayerPage(this);
    adminCoachPage = new AdminCoachPage(this);
    adminTournamentPage = new AdminTournamentPage(this);
    adminMatchPage = new AdminMatchPage(this);
    adminPlayerStatsPage = new AdminPlayerStatsPage(this); // Создаем страницу статистики игроков

    // Добавляем страницы в QStackedWidget
    //ui->stackedWidget->addWidget(loginPage);
    ui->stackedWidget->addWidget(adminClubPage);
    ui->stackedWidget->addWidget(adminPlayerPage);
    ui->stackedWidget->addWidget(adminCoachPage);
    ui->stackedWidget->addWidget(adminTournamentPage);
    ui->stackedWidget->addWidget(adminMatchPage);
    ui->stackedWidget->addWidget(adminPlayerStatsPage); // Добавляем страницу статистики игроков

    // Показываем по умолчанию страницу клубов
    ui->stackedWidget->setCurrentWidget(adminClubPage);

    // Обработка нажатий кнопок меню
    connect(ui->clubButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(adminClubPage);
    });
    connect(ui->playerButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(adminPlayerPage);
    });
    connect(ui->coachButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(adminCoachPage);
    });
    connect(ui->tournamentButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(adminTournamentPage);
    });
    connect(ui->matchButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(adminMatchPage);
    });
    connect(ui->playerstatsButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(adminPlayerStatsPage); // Переход к странице статистики игроков
    });
    connect(ui->logoutButton, &QPushButton::clicked, this, [=]() {
        // ui->stackedWidget->setCurrentWidget(loginPage);
        LoginPage *loginPage = new LoginPage();
        loginPage->show(); // Показываем страницу входа
        this->close(); // Закрываем главное окно администратора при выходе
    });
}

AdminMainWindow::~AdminMainWindow()
{
    delete ui;
}

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "clubpage.h"
#include "playerpage.h"
#include "coachpage.h"
#include "tournamentpage.h" // Подключаем TournamentPage
#include "loginpage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Создание страниц
    clubPage = new ClubPage(this);
    playerPage = new PlayerPage(this);
    coachPage = new CoachPage(this);
    tournamentPage = new TournamentPage(this); // Создаём страницу турниров

    // Добавляем страницы в QStackedWidget
    ui->stackedWidget->addWidget(clubPage);
    ui->stackedWidget->addWidget(playerPage);
    ui->stackedWidget->addWidget(coachPage);
    ui->stackedWidget->addWidget(tournamentPage); // Добавляем TournamentPage

    // Показываем по умолчанию страницу клубов
    ui->stackedWidget->setCurrentWidget(clubPage);

    // Обработка нажатий кнопок меню
    connect(ui->clubButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(clubPage);
    });
    connect(ui->playerButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(playerPage);
    });
    connect(ui->coachButton, &QPushButton::clicked, this, [=]() {
        ui->stackedWidget->setCurrentWidget(coachPage);
    });
    connect(ui->tournamentButton, &QPushButton::clicked, this, [=]() { // Добавляем обработку кнопки турниров
        ui->stackedWidget->setCurrentWidget(tournamentPage);
    });
    connect(ui->logoutButton, &QPushButton::clicked, this, [=]() {
        LoginPage *loginPage = new LoginPage();
        loginPage->show(); // Показываем страницу входа
        this->close(); // Закрываем главное окно при выходе
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

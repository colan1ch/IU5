#include "loginpage.h"
#include "ui_loginpage.h"
#include "mainwindow.h"
#include "adminmainwindow.h"

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginPage::onLoginButtonClicked);
}

LoginPage::~LoginPage() { delete ui; }

void LoginPage::onLoginButtonClicked()
{
    QString login = ui->loginEdit->text().trimmed();
    QString password = ui->passwordEdit->text().trimmed();

    if (login == "user" && password == "123") {
        MainWindow *clubPage = new MainWindow();
        clubPage->show();
        this->close();
    } else if (login == "admin" && password == "zxc") {
        AdminMainWindow *adminPage = new AdminMainWindow();
        adminPage->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Ошибка входа", "Неверный логин или пароль!");
    }
}

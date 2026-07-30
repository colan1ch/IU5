#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QMessageBox>
#include "clubpage.h"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

private slots:
    void onLoginButtonClicked(); // Обработка кнопки "Войти"

private:
    Ui::LoginPage *ui;
};

#endif // LOGINPAGE_H

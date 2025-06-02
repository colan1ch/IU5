#include "databasemanager.h"
#include "loginpage.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager dbManager;
    if (!dbManager.openConnection()) return -1;

    LoginPage w;
    w.show();
    return a.exec();
}

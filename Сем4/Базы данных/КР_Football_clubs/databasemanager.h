// File: databasemanager.h
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    bool openConnection();
    void closeConnection();
private:
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H

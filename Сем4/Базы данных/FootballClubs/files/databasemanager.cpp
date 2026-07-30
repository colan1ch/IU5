#include "databasemanager.h"
#include <QtSql/QSqlError>

DatabaseManager::DatabaseManager()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("football_clubs");
    db.setHostName("localhost");
    db.setUserName("postgres");
    db.setPassword("1201");
    db.setPort(5433);
}

DatabaseManager::~DatabaseManager()
{
    closeConnection();
}

bool DatabaseManager::openConnection()
{
    if (!db.open()) {
        qCritical() << "Database error:" << db.lastError().text();
        return false;
    }
    return true;
}

void DatabaseManager::closeConnection()
{
    if (db.isOpen()) db.close();
}


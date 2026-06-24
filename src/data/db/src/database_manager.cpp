#include "database_manager.hpp"

#include <QDebug>
#include <QSqlError>

DatabaseManager::DatabaseManager() {
    db_ = QSqlDatabase::addDatabase("QPSQL");
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect(
    const QString& host,
    int port,
    const QString& database_name,
    const QString& username,
    const QString& password) {

    db_.setHostName(host);
    db_.setPort(port);
    db_.setDatabaseName(database_name);
    db_.setUserName(username);
    db_.setPassword(password);

    if (!db_.open()) {
        qWarning()
            << "Postgres connection error:"
            << db_.lastError().text();

        return false;
    }

    return true;
}

void DatabaseManager::disconnect() {
    if (db_.isOpen()) {
        db_.close();
    }
}

QSqlDatabase DatabaseManager::database() const {
    return db_;
}
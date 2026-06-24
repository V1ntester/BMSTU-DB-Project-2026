#pragma once

#include <QSqlDatabase>
#include <QString>

class DatabaseManager {
public:
    DatabaseManager();

    ~DatabaseManager();

    bool connect(
        const QString& host,
        int port,
        const QString& database_name,
        const QString& username,
        const QString& password);

    void disconnect();

    QSqlDatabase database() const;

private:
    QSqlDatabase db_;
};
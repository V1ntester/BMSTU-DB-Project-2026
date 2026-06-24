#pragma once

#include <QDateTime>
#include <QString>

struct User {
    long id = 0;
    long role_id = 0;
    QString role_name;
    long position_id = 0;
    QString position_name;
    long team_id = 0;
    QString team_name;

    QString first_name;
    QString last_name;

    QString email;
    QString phone;

    QString password_hash;

    QDateTime created_at;
    QDateTime updated_at;
};
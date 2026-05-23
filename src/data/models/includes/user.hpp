#pragma once

#include <QDateTime>
#include <QString>

struct User {
    long id = 0;
    long role_id = 0;
    long position_id = 0;
    long team_id = 0;

    QString first_name;
    QString last_name;

    QString email;
    QString phone;

    QString password_hash;

    QDateTime created_at;
    QDateTime updated_at;
};
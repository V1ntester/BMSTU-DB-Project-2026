#pragma once

#include <QDateTime>
#include <QString>

struct Position {
    long id = 0;
    
    QString name;

    QDateTime created_at;
    QDateTime updated_at;
};
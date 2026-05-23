#pragma once

#include <QDateTime>
#include <QString>

struct Role
{
    long id = 0;
    
    QString name;

    QDateTime created_at;
    QDateTime updated_at;
};

#pragma once

#include <QDateTime>
#include <QString>

struct Review {
    long id = 0;
    long sprint_id = 0;
    
    QString name;
    QString description;

    QDateTime created_at;
    QDateTime updated_at;
};
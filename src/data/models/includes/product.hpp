#pragma once

#include <QDateTime>
#include <QString>

struct Product {
    long id = 0;
    
    QString name;
    QString description;

    QDateTime created_at;
    QDateTime updated_at;
};
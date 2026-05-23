#pragma once

#include <QDateTime>
#include <QString>

struct ProductBacklogItem {
    long id = 0;
    long product_id = 0;

    QString name;
    QString description;

    int story_points = 0;
    
    QDateTime created_at;
    QDateTime updated_at;
};
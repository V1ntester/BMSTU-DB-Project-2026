#pragma once

#include <QDateTime>
#include <QString>

struct TeamProduct {
    long team_id = 0;
    QString team_name;
    long product_id = 0;
    QString product_name;

    QDateTime created_at;
};
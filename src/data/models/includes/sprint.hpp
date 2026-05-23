#pragma once

#include <QDateTime>
#include <QString>

struct Sprint {
    long id = 0;
    long product_id = 0;

    QString name;
    QString goal;

    QDateTime start_time;
    QDateTime end_time;

    QDateTime created_at;
    QDateTime updated_at;
};
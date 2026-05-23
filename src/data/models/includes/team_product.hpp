#pragma once

#include <QDateTime>

struct TeamProduct {
    long team_id = 0;
    long product_id = 0;

    QDateTime created_at;
};
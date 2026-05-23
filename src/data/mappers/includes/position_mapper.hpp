#pragma once

#include <QSqlQuery>

#include "position.hpp"

class PositionMapper {
    public:
        static Position from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const Position &position);
        static void bind_update(QSqlQuery &query, const Position &position);      
};
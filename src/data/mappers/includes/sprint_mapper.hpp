#pragma once

#include <QSqlQuery>

#include "sprint.hpp"

class SprintMapper {
    public:
        static Sprint from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const Sprint &sprint);
        static void bind_update(QSqlQuery &query, const Sprint &sprint);
};
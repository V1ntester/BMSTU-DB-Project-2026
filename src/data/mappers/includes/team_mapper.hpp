#pragma once

#include <QSqlQuery>

#include "team.hpp"

class TeamMapper{
    public:
        static Team from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const Team &team);
        static void bind_update(QSqlQuery &query, const Team &team);
};
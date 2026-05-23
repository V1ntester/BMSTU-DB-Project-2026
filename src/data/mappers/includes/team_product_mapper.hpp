#pragma once

#include <QSqlQuery>

#include "team_product.hpp"

class TeamProductMapper {
    public:
        static TeamProduct from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const TeamProduct &team_product);
};
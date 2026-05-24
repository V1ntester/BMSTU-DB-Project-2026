#pragma once

#include <QSqlQuery>

#include "team_product.hpp"

class TeamProductMapper {
    public:
        static TeamProduct from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const TeamProduct &team_product);
        static void bind_remove(QSqlQuery &query, long team_id, long product_id);
        static void bind_find_by_ids(QSqlQuery &query, long team_id, long product_id);
};
#pragma once

#include <QSqlQuery>

#include "product.hpp"

class ProductMapper {
    public:
        static Product from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const Product &product);
        static void bind_update(QSqlQuery &query, const Product &product);
};
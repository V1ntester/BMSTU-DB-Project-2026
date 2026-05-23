#pragma once

#include <QSqlQuery>

#include "product_backlog_item.hpp"

class ProductBacklogItemMapper {
    public:
        static ProductBacklogItem from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const ProductBacklogItem &product_backlog_item);
        static void bind_update(QSqlQuery &query, const ProductBacklogItem &product_backlog_item);
};
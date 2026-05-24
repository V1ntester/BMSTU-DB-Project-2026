#pragma once

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "product_backlog_item.hpp"
#include "repository.hpp"

class ProductBacklogItemRepository : public Repository {
    public:
        explicit ProductBacklogItemRepository(QSqlDatabase db);

        void insert(const ProductBacklogItem &product_backlog_item);
        void update(const ProductBacklogItem &product_backlog_item);
        void remove(long id);

        std::optional<ProductBacklogItem> find_by_id(long id);
        std::vector<ProductBacklogItem> find_all();
};
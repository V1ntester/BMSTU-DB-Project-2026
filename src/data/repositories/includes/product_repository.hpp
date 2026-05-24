#pragma once

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "product.hpp"
#include "repository.hpp"

class ProductRepository : public Repository {
    public:
        explicit ProductRepository(QSqlDatabase db);

        void insert(const Product &product);
        void update(const Product &product);
        void remove(long id);

        std::optional<Product> find_by_id(long id);
        std::vector<Product> find_all();
};
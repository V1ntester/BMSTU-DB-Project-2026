#pragma once 

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "team_product.hpp"

class TeamProductRepository : public Repository {
    public:
        explicit TeamProductRepository(QSqlDatabase db);
    
        void insert(const TeamProduct &team_product);
        void remove(long product_id, long team_id);

        std::optional<TeamProduct> find_by_ids(long product_id, long team_id);
        std::vector<TeamProduct> find_all();
};
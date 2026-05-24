#pragma once 

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "increment.hpp"
#include "repository.hpp"

class IncrementRepository : public Repository {
    public:
        explicit IncrementRepository(QSqlDatabase db);
    
        void insert(const Increment &increment);
        void update(const Increment &increment);
        void remove(long id);

        std::optional<Increment> find_by_id(long id);
        std::vector<Increment> find_all();
};
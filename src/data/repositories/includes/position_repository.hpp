#pragma once 

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "position.hpp"
#include "repository.hpp"

class PositionRepository : public Repository {
    public:
        explicit PositionRepository(QSqlDatabase db);
    
        void insert(const Position &position);
        void update(const Position &position);
        void remove(long id);

        std::optional<Position> find_by_id(long id);
        std::vector<Position> find_all();
};
#pragma once

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "sprint.hpp"

class SprintRepository : public Repository {
    public:
        explicit SprintRepository(QSqlDatabase db);

        void insert(const Sprint &sprint);
        void update(const Sprint &sprint);
        void remove(long id);

        std::optional<Sprint> find_by_id(long id);
        std::vector<Sprint> find_all();
};
#pragma once 

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "team.hpp"

class TeamRepository : public Repository {
    public:
        explicit TeamRepository(QSqlDatabase db);
    
        void insert(const Team &team);
        void update(const Team &team);
        void remove(long id);

        std::optional<Team> find_by_id(long id);
        std::vector<Team> find_all();
};
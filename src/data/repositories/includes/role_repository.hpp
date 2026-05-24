#pragma once 

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "role.hpp"

class RoleRepository : public Repository {
    public:
        explicit RoleRepository(QSqlDatabase db);
    
        void insert(const Role &role);
        void update(const Role &role);
        void remove(long id);

        std::optional<Role> find_by_id(long id);
        std::vector<Role> find_all();
};
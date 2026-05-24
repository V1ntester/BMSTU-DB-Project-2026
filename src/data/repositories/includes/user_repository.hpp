#pragma once 

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "user.hpp"

class UserRepository : public Repository {
    public:
        explicit UserRepository(QSqlDatabase db);
    
        void insert(const User &user);
        void update(const User &user);
        void remove(long id);

        std::optional<User> find_by_id(long id);
        std::vector<User> find_all();
};
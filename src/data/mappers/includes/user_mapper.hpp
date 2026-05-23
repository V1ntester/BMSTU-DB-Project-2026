#pragma once

#include <QSqlQuery>

#include "user.hpp"

class UserMapper {
    public:
        static User from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const User &user);
        static void bind_update(QSqlQuery &query, const User &user);
};
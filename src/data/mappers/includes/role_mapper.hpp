#pragma once

#include <QSqlQuery>

#include "role.hpp"

class RoleMapper {
    public:
        static Role from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const Role &role);
        static void bind_update(QSqlQuery &query, const Role &role);      
};
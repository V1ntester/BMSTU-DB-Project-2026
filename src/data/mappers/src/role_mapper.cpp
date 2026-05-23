#include "role_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

Role RoleMapper::from_query(const QSqlQuery &query)
{
    Role role;

    role.id = query.value("id").toLongLong();
    
    role.name = query.value("name").toString();

    role.created_at = query.value("created_at").toDateTime();
    role.updated_at = query.value("updated_at").toDateTime();

    return role;
}

void RoleMapper::bind_insert(QSqlQuery &query, const Role &role)
{
    query.bindValue(":name", QVariant::fromValue(role.name));
}

void RoleMapper::bind_update(QSqlQuery &query, const Role &role)
{
    query.bindValue(":id", QVariant::fromValue(role.id));
    query.bindValue(":name", QVariant::fromValue(role.name));
}

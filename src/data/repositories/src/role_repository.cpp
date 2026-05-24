#include "role_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "role.hpp"
#include "role_mapper.hpp"

RoleRepository::RoleRepository(QSqlDatabase db) : Repository(db)
{
}

void RoleRepository::insert(const Role &role)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO roles (
            name
        )
        VALUES (
            :name
        )
    )");

    RoleMapper::bind_insert(query, role);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void RoleRepository::update(const Role &role)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE roles
        SET
            name = :name
        WHERE id = :id
    )");

    RoleMapper::bind_update(query, role);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void RoleRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM roles
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<Role> RoleRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM roles
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return RoleMapper::from_query(query);
}

std::vector<Role> RoleRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM roles
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<Role> roles;

    while (query.next()) {
        roles.push_back(RoleMapper::from_query(query));
    }

    return roles;
}

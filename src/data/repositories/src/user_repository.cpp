#include "user_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "user.hpp"
#include "user_mapper.hpp"

UserRepository::UserRepository(QSqlDatabase db) : Repository(db_)
{
}

void UserRepository::insert(const User &user)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO users (
            role_id,
            position_id,
            team_id,
            first_name,
            last_name,
            email,
            phone,
            password_hash
        )
        VALUES (
            :role_id,
            :position_id,
            :team_id,
            :first_name,
            :last_name,
            :email,
            :phone,
            :password_hash
        )
    )");

    UserMapper::bind_insert(query, user);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void UserRepository::update(const User &user)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE users
        SET
            role_id = :role_id,
            position_id = :position_id,
            team_id = :team_id,
            first_name = :first_name,
            last_name = :last_name,
            email = :email,
            phone = :phone,
            password_hash = :password_hash
        WHERE id = :id
    )");

    UserMapper::bind_insert(query, user);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void UserRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM users
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<User> UserRepository::find_by_id(long id)
{
   QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM users
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return  UserMapper::from_query(query);
}

std::vector<User> UserRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM users
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<User> users;

    while (query.next()) {
        users.push_back(UserMapper::from_query(query));
    }

    return users;
}

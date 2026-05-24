#include "team_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "team.hpp"
#include "team_mapper.hpp"

TeamRepository::TeamRepository(QSqlDatabase db) : Repository(db)
{
}

void TeamRepository::insert(const Team &team)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO teams (
            name
        )
        VALUES (
            :name
        )
    )");

    TeamMapper::bind_insert(query, team);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void TeamRepository::update(const Team &team)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE teams
        SET name = :name
        WHERE id = :id
    )");

    TeamMapper::bind_update(query, team);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }    
}

void TeamRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM teams
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<Team> TeamRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM teams
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return TeamMapper::from_query(query);
}

std::vector<Team> TeamRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM teams
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<Team> teams;

    while (query.next()) {
        teams.push_back(TeamMapper::from_query(query));
    }

    return teams;
}

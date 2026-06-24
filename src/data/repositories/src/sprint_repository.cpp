#include "sprint_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "sprint.hpp"
#include "sprint_mapper.hpp"

SprintRepository::SprintRepository(QSqlDatabase db) : Repository(db)
{
}

void SprintRepository::insert(const Sprint &sprint)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO sprints (
            product_id,
            name,
            goal,
            start_time,
            end_time
        )
        VALUES (
            :product_id,
            :name,
            :goal,
            :start_time,
            :end_time
        )
    )");

    SprintMapper::bind_insert(query, sprint);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void SprintRepository::update(const Sprint &sprint)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE sprints
        SET
            product_id = :product_id,
            name = :name,
            goal = :goal,
            start_time = :start_time,
            end_time = :end_time
        WHERE id = :id
    )");

    SprintMapper::bind_update(query, sprint);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void SprintRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM sprints
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }   
}

std::optional<Sprint> SprintRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT 
            s.id,
            s.product_id,
            s.name,
            s.goal,
            s.start_time,
            s.end_time,
            s.created_at,
            s.updated_at,
            p.name as product_name
        FROM sprints s
        JOIN products p
            ON p.id = s.product_id
        WHERE s.id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return SprintMapper::from_query(query);
}

std::vector<Sprint> SprintRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT 
            s.id,
            s.product_id,
            s.name,
            s.goal,
            s.start_time,
            s.end_time,
            s.created_at,
            s.updated_at,
            p.name as product_name
        FROM sprints s
        JOIN products p
            ON p.id = s.product_id
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<Sprint> sprints;

    while (query.next())
    {
        sprints.push_back(SprintMapper::from_query(query));
    }

    return sprints;
}

#include "increment_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "increment.hpp"
#include "increment_mapper.hpp"
#include "repository.hpp"

IncrementRepository::IncrementRepository(QSqlDatabase db) : Repository(db)
{
}

void IncrementRepository::insert(const Increment &increment)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO increments (
            user_id,
            sprint_backlog_item_id,
            note,
            completed_story_points
        )
        VALUES (
            :user_id,
            :sprint_backlog_item_id,
            :note,
            :completed_story_points
        )
    )");

    IncrementMapper::bind_insert(query, increment);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void IncrementRepository::update(const Increment &increment)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE increments
        SET
            user_id = :user_id,
            sprint_backlog_item_id = :sprint_backlog_item_id,
            note = :note,
            completed_story_points = :completed_story_points
        WHERE id = :id
    )");

    IncrementMapper::bind_update(query, increment);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void IncrementRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM increments
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<Increment> IncrementRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT 
            i.id,
            i.user_id,
            i.sprint_backlog_item_id,
            i.note,
            i.completed_story_points,
            i.created_at,
            i.updated_at,
            u.email AS user_email,
            sbi.name AS sprint_backlog_item_name
        FROM increments i
        JOIN users u 
            ON u.id = i.user_id
        JOIN sprint_backlog_items sbi 
            ON sbi.id = i.sprint_backlog_item_id
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return IncrementMapper::from_query(query);
}

std::vector<Increment> IncrementRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT 
            i.id,
            i.user_id,
            i.sprint_backlog_item_id,
            i.note,
            i.completed_story_points,
            i.created_at,
            i.updated_at,
            u.email AS user_email,
            sbi.name AS sprint_backlog_item_name
        FROM increments i
        JOIN users u 
            ON u.id = i.user_id
        JOIN sprint_backlog_items sbi 
            ON sbi.id = i.sprint_backlog_item_id
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
    
    std::vector<Increment> increments;

    while (query.next()) {
        increments.push_back(IncrementMapper::from_query(query));
    }

    return increments;
}

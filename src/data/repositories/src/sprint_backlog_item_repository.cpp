#include "sprint_backlog_item_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "sprint_backlog_item.hpp"
#include "sprint_backlog_item_mapper.hpp"

SprintBacklogItemRepository::SprintBacklogItemRepository(QSqlDatabase db) : Repository(db)
{
}

void SprintBacklogItemRepository::insert(const SprintBacklogItem &sprint_backlog_item)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO sprint_backlog_items (
            product_backlog_item_id,
            sprint_id,
            name,
            description,
            story_points,
            priority,
            status
        )
        VALUES (
            :product_backlog_item_id,
            :sprint_id,
            :name,
            :description,
            :story_points,
            :priority,
            :status
        )
    )");

    SprintBacklogItemMapper::bind_insert(query, sprint_backlog_item);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void SprintBacklogItemRepository::update(const SprintBacklogItem &sprint_backlog_item)
{
    QSqlQuery query(db_);

    query.prepare(R"(
            UPDATE sprint_backlog_items
            SET
                product_backlog_item_id = :product_backlog_item_id,
                sprint_id = :sprint_id,
                name = :name,
                description = :description,
                story_points = :story_points,
                priority = :priority,
                status = :status,
            WHERE id = :id
    )");

    SprintBacklogItemMapper::bind_update(query, sprint_backlog_item);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void SprintBacklogItemRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM sprint_backlog_items
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<SprintBacklogItem> SprintBacklogItemRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT 
            sbi.id,
            sbi.product_backlog_item_id,
            sbi.sprint_id,
            sbi.name,
            sbi.description,
            sbi.story_points,
            sbi.priority,
            sbi.status,
            sbi.created_at,
            sbi.updated_at,
            pbi.name as product_backlog_item_name,
            s.name as sprint_name
        FROM sprint_backlog_items sbi
        JOIN product_backlog_items pbi
            ON pbi.id = sbi.product_backlog_item_id
        JOIN sprints s
            ON s.id = sbi.sprint_id
        WHERE sbi.id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return SprintBacklogItemMapper::from_query(query);
}

std::vector<SprintBacklogItem> SprintBacklogItemRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT 
            sbi.id,
            sbi.product_backlog_item_id,
            sbi.sprint_id,
            sbi.name,
            sbi.description,
            sbi.story_points,
            sbi.priority,
            sbi.status,
            sbi.created_at,
            sbi.updated_at,
            pbi.name as product_backlog_item_name,
            s.name as sprint_name
        FROM sprint_backlog_items sbi
        JOIN product_backlog_items pbi
            ON pbi.id = sbi.product_backlog_item_id
        JOIN sprints s
            ON s.id = sbi.sprint_id
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<SprintBacklogItem> sprint_backlog_items;

    while (query.next()) {
        sprint_backlog_items.push_back(SprintBacklogItemMapper::from_query(query));
    }

    return sprint_backlog_items;
}

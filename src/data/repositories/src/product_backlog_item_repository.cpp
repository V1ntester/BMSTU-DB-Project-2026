#include "product_backlog_item_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "product_backlog_item.hpp"
#include "product_backlog_item_mapper.hpp"
#include "repository.hpp"

ProductBacklogItemRepository::ProductBacklogItemRepository(QSqlDatabase db) : Repository(db)
{
}

void ProductBacklogItemRepository::insert(const ProductBacklogItem &product_backlog_item)
{
    QSqlQuery query(db_);
    
    query.prepare(R"(
        INSERT INTO product_backlog_items (
            product_id,
            name,
            description,
            story_points
        )
        VALUES (
            :product_id,
            :name,
            :description,
            :story_points
        )
    )");

    ProductBacklogItemMapper::bind_insert(query, product_backlog_item);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void ProductBacklogItemRepository::update(const ProductBacklogItem &product_backlog_item)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE product_backlog_items
        SET
            product_id = :product_id,
            name = :name,
            description = :description
            story_points = :story_points
        WHERE id = :id
    )");

    ProductBacklogItemMapper::bind_update(query, product_backlog_item);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void ProductBacklogItemRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM product_backlog_items
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<ProductBacklogItem> ProductBacklogItemRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT 
            pbi.id,
            pbi.product_id,
            pbi.name,
            pbi.description,
            pbi.story_points,
            pbi.created_at,
            pbi.updated_at,
            p.name as product_name
        FROM product_backlog_items pbi
        JOIN products p
            ON p.id = pbi.product_id
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return ProductBacklogItemMapper::from_query(query);
}

std::vector<ProductBacklogItem> ProductBacklogItemRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT
            pbi.id,
            pbi.product_id,
            pbi.name,
            pbi.description,
            pbi.story_points,
            pbi.created_at,
            pbi.updated_at,
            p.name as product_name
        FROM product_backlog_items pbi
        JOIN products p
            ON p.id = pbi.product_id
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<ProductBacklogItem> product_backlog_items;

    while (query.next()) {
        product_backlog_items.push_back(ProductBacklogItemMapper::from_query(query));
    }

    return product_backlog_items;
}

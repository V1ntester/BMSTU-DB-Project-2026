#include "product_backlog_item_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "product_backlog_item.hpp"

ProductBacklogItem ProductBacklogItemMapper::from_query(const QSqlQuery &query)
{
    ProductBacklogItem product_backlog_item;

    product_backlog_item.id = query.value("id").toLongLong();    
    product_backlog_item.product_id = query.value("product_id").toLongLong();
    
    product_backlog_item.name = query.value("name").toString();
    product_backlog_item.description = query.value("description").toString();

    product_backlog_item.story_points = query.value("story_points").toInt();

    product_backlog_item.created_at = query.value("created_at").toDateTime();
    product_backlog_item.updated_at = query.value("updated_at").toDateTime();   

    return product_backlog_item;
}

void ProductBacklogItemMapper::bind_insert(QSqlQuery &query, const ProductBacklogItem &product_backlog_item)
{
    query.bindValue(":product_id", QVariant::fromValue(product_backlog_item.product_id));
    query.bindValue("name", QVariant::fromValue(product_backlog_item.name));
    query.bindValue("description", QVariant::fromValue(product_backlog_item.description));
    query.bindValue("story_points", QVariant::fromValue(product_backlog_item.story_points));    
}

void ProductBacklogItemMapper::bind_update(QSqlQuery &query, const ProductBacklogItem &product_backlog_item)
{
    query.bindValue(":id", QVariant::fromValue(product_backlog_item.id));
    query.bindValue(":product_id", QVariant::fromValue(product_backlog_item.product_id));
    query.bindValue("name", QVariant::fromValue(product_backlog_item.name));
    query.bindValue("description", QVariant::fromValue(product_backlog_item.description));
    query.bindValue("story_points", QVariant::fromValue(product_backlog_item.story_points));   
}

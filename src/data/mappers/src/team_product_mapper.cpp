#include "team_product_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "team_product.hpp"

TeamProduct TeamProductMapper::from_query(const QSqlQuery &query)
{
    TeamProduct team_product;
    team_product.team_id = query.value("team_id").toLongLong();
    team_product.product_id = query.value("product_id").toLongLong();
    team_product.created_at = query.value("created_at").toDateTime();
    return team_product;
}

void TeamProductMapper::bind_insert(QSqlQuery &query, const TeamProduct &team_product)
{
    query.bindValue(":team_id", QVariant::fromValue(team_product.team_id));
    query.bindValue(":product_id", QVariant::fromValue(team_product.product_id));
}

void TeamProductMapper::bind_remove(QSqlQuery &query, long team_id, long product_id) {
    query.bindValue(":team_id", QVariant::fromValue(team_id));
    query.bindValue(":product_id", QVariant::fromValue(product_id));
}

void TeamProductMapper::bind_find_by_ids(QSqlQuery &query, long team_id, long product_id) {
    query.bindValue(":team_id", QVariant::fromValue(team_id));
    query.bindValue(":product_id", QVariant::fromValue(product_id));
}
#include "product_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "product.hpp"

Product ProductMapper::from_query(const QSqlQuery &query)
{
    Product product;

    product.id = query.value("id").toLongLong();
    
    product.name = query.value("name").toString();
    product.description = query.value("description").toString();

    product.created_at = query.value("created_at").toDateTime();
    product.updated_at = query.value("updated_at").toDateTime();

    return product;
}

void ProductMapper::bind_insert(QSqlQuery &query, const Product &product)
{
    query.bindValue(":name", QVariant::fromValue(product.name));
    query.bindValue(":description", QVariant::fromValue(product.description));
}

void ProductMapper::bind_update(QSqlQuery &query, const Product &product)
{
    query.bindValue(":id", QVariant::fromValue(product.id));
    query.bindValue(":name", QVariant::fromValue(product.name));
    query.bindValue(":description", QVariant::fromValue(product.description));
}

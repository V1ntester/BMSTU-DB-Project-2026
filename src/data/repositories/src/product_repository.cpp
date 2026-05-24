#include "product_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "product.hpp"
#include "product_mapper.hpp"
#include "repository.hpp"

ProductRepository::ProductRepository(QSqlDatabase db) : Repository(db)
{
}

void ProductRepository::insert(const Product &product)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO products (
            name,
            description
        )
        VALUES (
            :name,
            :description
        )
    )");

    ProductMapper::bind_insert(query, product);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

}

void ProductRepository::update(const Product &product)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE products
        SET 
            name = :name,
            description = :description
        WHERE id = :id
    )");

    ProductMapper::bind_update(query, product);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void ProductRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM products
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<Product> ProductRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM products
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return ProductMapper::from_query(query);

}

std::vector<Product> ProductRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM products
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<Product> products;

    while (query.next()) {
        products.push_back(ProductMapper::from_query(query));
    }

    return products;
}

#include "team_product_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "team_product.hpp"
#include "team_product_mapper.hpp"

TeamProductRepository::TeamProductRepository(QSqlDatabase db) : Repository(db_)
{
}

void TeamProductRepository::insert(const TeamProduct &team_product)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO team_products (
            team_id,
            product_id
        )
        VALUES (
            :team_id,
            :product_id
        )
    )");

    TeamProductMapper::bind_insert(query, team_product);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void TeamProductRepository::remove(long team_id, long product_id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM team_products
        WHERE
            team_id = :team_id AND 
            product_id = :product_id
    )");

    TeamProductMapper::bind_remove(query, team_id, product_id);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<TeamProduct> TeamProductRepository::find_by_ids(long product_id, long team_id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM team_products
        WHERE
            team_id = :team_id AND 
            product_id = :product_id
    )");

    TeamProductMapper::bind_find_by_ids(query, team_id, product_id);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return TeamProductMapper::from_query(query);
}

std::vector<TeamProduct> TeamProductRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM team_products
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<TeamProduct> team_products;

    while (query.next())
    {
        team_products.push_back(TeamProductMapper::from_query(query));
    }

    return team_products;
}

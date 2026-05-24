#include "review_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "review.hpp"
#include "review_mapper.hpp"

ReviewRepository::ReviewRepository(QSqlDatabase db) : Repository(db)
{
}

void ReviewRepository::insert(const Review &review)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO reviews (
            sprint_id,
            name,
            description
        )
        VALUES (
            :sprint_id,
            :name,
            :description
        )
    )");

    ReviewMapper::bind_insert(query, review);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void ReviewRepository::update(const Review &review)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE reviews
        SET
            sprint_id = :sprint_id,
            name = :name,
            description = :description
        WHERE id = :id    
    )");

    ReviewMapper::bind_update(query, review);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void ReviewRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM reviews
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<Review> ReviewRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM reviews
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return ReviewMapper::from_query(query);
}

std::vector<Review> ReviewRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM reviews
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<Review> reviews;

    while (query.next()) {
        reviews.push_back(ReviewMapper::from_query(query));
    }

    return reviews;
}

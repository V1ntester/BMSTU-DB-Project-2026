#include "review_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "review.hpp"

Review ReviewMapper::from_query(const QSqlQuery &query)
{
    Review review;

    review.id = query.value("id").toLongLong();
    review.sprint_id = query.value("sprint_id").toLongLong();

    review.name = query.value("name").toString();
    review.description = query.value("description").toString();

    review.created_at = query.value("created_at").toDateTime();
    review.updated_at = query.value("updated_at").toDateTime();

    return review;
}

void ReviewMapper::bind_insert(QSqlQuery &query, const Review &review)
{
    query.bindValue(":sprint_id", QVariant::fromValue(review.sprint_id));
    query.bindValue(":name", QVariant::fromValue(review.name));
    query.bindValue(":description", QVariant::fromValue(review.description));
}

void ReviewMapper::bind_update(QSqlQuery &query, const Review &review)
{
    query.bindValue(":id", QVariant::fromValue(review.id));
    query.bindValue(":sprint_id", QVariant::fromValue(review.sprint_id));
    query.bindValue(":name", QVariant::fromValue(review.name));
    query.bindValue(":description", QVariant::fromValue(review.description));
}

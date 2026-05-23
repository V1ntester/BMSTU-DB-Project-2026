#include "increment_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "increment.hpp"

Increment IncrementMapper::from_query(const QSqlQuery &query)
{
    Increment increment;

    increment.id = query.value("id").toLongLong();
    increment.user_id = query.value("user_id").toLongLong();
    increment.sprint_backlog_item_id = query.value("sprint_backlog_item_id").toLongLong();

    increment.note = query.value("note").toString();

    increment.completed_story_points = query.value("completed_story_points").toInt();

    increment.created_at = query.value("created_at").toDateTime();
    increment.updated_at = query.value("updated_at").toDateTime();

    return increment;
}

void IncrementMapper::bind_insert(QSqlQuery &query, const Increment &increment)
{
    query.bindValue(":user_id", QVariant::fromValue(increment.user_id));
    query.bindValue(":sprint_backlog_item_id", QVariant::fromValue(increment.sprint_backlog_item_id));
    query.bindValue(":note", QVariant::fromValue(increment.note));
    query.bindValue(":completed_story_points", QVariant::fromValue(increment.completed_story_points));
}

void IncrementMapper::bind_update(QSqlQuery &query, const Increment &increment)
{
    query.bindValue(":id", QVariant::fromValue(increment.id));
    query.bindValue(":user_id", QVariant::fromValue(increment.user_id));
    query.bindValue(":sprint_backlog_item_id", QVariant::fromValue(increment.sprint_backlog_item_id));
    query.bindValue(":note", QVariant::fromValue(increment.note));
    query.bindValue(":completed_story_points", QVariant::fromValue(increment.completed_story_points));
}

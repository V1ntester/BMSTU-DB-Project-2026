#include "position_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "position.hpp"

Position PositionMapper::from_query(const QSqlQuery &query)
{
    Position position;

    position.id = query.value("id").toLongLong();

    position.name = query.value("name").toString();

    position.created_at = query.value("created_at").toDateTime();
    position.updated_at = query.value("updated_at").toDateTime();

    return position;
}

void PositionMapper::bind_insert(QSqlQuery &query, const Position &position)
{
    query.bindValue(":name", QVariant::fromValue(position.name));
}

void PositionMapper::bind_update(QSqlQuery &query, const Position &position)
{
    query.bindValue(":id", QVariant::fromValue(position.id));
    query.bindValue(":name", QVariant::fromValue(position.name));
}

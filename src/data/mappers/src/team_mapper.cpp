#include "team_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "team.hpp"

Team TeamMapper::from_query(const QSqlQuery &query)
{
    Team team;

    team.id = query.value("id").toLongLong();

    team.name = query.value("name").toString();

    team.created_at = query.value("created_at").toDateTime();
    team.updated_at = query.value("updated_at").toDateTime();

    return team;
}

void TeamMapper::bind_insert(QSqlQuery &query, const Team &team)
{
    query.bindValue(":name", QVariant::fromValue(team.name));
}

void TeamMapper::bind_update(QSqlQuery &query, const Team &team)
{
    query.bindValue(":id", QVariant::fromValue(team.id));
    query.bindValue(":name", QVariant::fromValue(team.name));
}

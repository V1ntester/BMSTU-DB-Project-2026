#include "sprint_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "sprint.hpp"

Sprint SprintMapper::from_query(const QSqlQuery &query)
{
    Sprint sprint;

    sprint.id = query.value("id").toLongLong();

    sprint.product_id = query.value("product_id").toLongLong();
    sprint.product_name = query.value("product_name").toString();

    sprint.name = query.value("name").toString();
    sprint.goal = query.value("goal").toString();

    sprint.start_time = query.value("start_time").toDateTime();
    sprint.end_time = query.value("end_time").toDateTime();

    sprint.created_at = query.value("created_at").toDateTime();
    sprint.updated_at = query.value("updated_at").toDateTime();

    return sprint;
}

void SprintMapper::bind_insert(QSqlQuery &query, const Sprint &sprint)
{
    query.bindValue(":product_id", QVariant::fromValue(sprint.product_id));    
    query.bindValue(":name", QVariant::fromValue(sprint.name));
    query.bindValue(":goal", QVariant::fromValue(sprint.goal));
    query.bindValue(":start_time", QVariant::fromValue(sprint.start_time));
    query.bindValue(":end_time", QVariant::fromValue(sprint.end_time));    
}

void SprintMapper::bind_update(QSqlQuery &query, const Sprint &sprint)
{
    query.bindValue(":id", QVariant::fromValue(sprint.id));
    query.bindValue(":product_id", QVariant::fromValue(sprint.product_id));    

    query.bindValue(":name", QVariant::fromValue(sprint.name));
    query.bindValue(":goal", QVariant::fromValue(sprint.goal));
    
    query.bindValue(":start_time", QVariant::fromValue(sprint.start_time));
    query.bindValue(":end_time", QVariant::fromValue(sprint.end_time));   
}

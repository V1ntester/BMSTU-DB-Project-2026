#include "sprint_backlog_item_mapper.hpp"

#include <QSqlQuery>
#include <QString>
#include <QVariant>

#include "sprint_backlog_item.hpp"

SprintBacklogItem SprintBacklogItemMapper::from_query(const QSqlQuery &query)
{
    SprintBacklogItem sprint_backlog_item;

    sprint_backlog_item.id = query.value("id").toLongLong();

    sprint_backlog_item.product_backlog_item_id = query.value("product_backlog_item_id").toLongLong();
    sprint_backlog_item.product_backlog_item_name = query.value("product_backlog_item_name").toString();

    sprint_backlog_item.sprint_id = query.value("sprint_id").toLongLong();
    sprint_backlog_item.sprint_name = query.value("sprint_name").toString();

    sprint_backlog_item.name = query.value("name").toString();
    sprint_backlog_item.description = query.value("description").toString();

    sprint_backlog_item.story_points = query.value("story_points").toInt();

    QString priority = query.value("priority").toString();

    if (priority == "Blocker") {
        sprint_backlog_item.priority = PriorityLevel::kBlocker;
    } else if (priority == "High") {
        sprint_backlog_item.priority = PriorityLevel::kHigh;
    } else if (priority == "Medium") {
        sprint_backlog_item.priority = PriorityLevel::kMedium;
    } else {
        sprint_backlog_item.priority = PriorityLevel::kLow;
    }

    QString status = query.value("status").toString();

    if (status == "To Do") {
        sprint_backlog_item.status = TaskStatus::kToDo;
    } else if (status == "In Progress") {
        sprint_backlog_item.status = TaskStatus::kInProgress;
    } else if (status == "In Review") {
        sprint_backlog_item.status = TaskStatus::kInReview;
    } else {
        sprint_backlog_item.status = TaskStatus::kDone;
    }

    sprint_backlog_item.created_at = query.value("created_at").toDateTime();
    sprint_backlog_item.updated_at = query.value("updated_at").toDateTime();

    return sprint_backlog_item;
}

void SprintBacklogItemMapper::bind_insert(QSqlQuery &query, const SprintBacklogItem &sprint_backlog_item)
{
    query.bindValue(":product_backlog_item_id", QVariant::fromValue(sprint_backlog_item.product_backlog_item_id));
    query.bindValue(":sprint_id", QVariant::fromValue(sprint_backlog_item.sprint_id));

    query.bindValue(":name", QVariant::fromValue(sprint_backlog_item.name));
    query.bindValue(":description", QVariant::fromValue(sprint_backlog_item.description));

    query.bindValue(":story_points", QVariant::fromValue(sprint_backlog_item.story_points));    

    if (sprint_backlog_item.priority == PriorityLevel::kBlocker) {
        query.bindValue(":priority", "Blocker");
    } else if (sprint_backlog_item.priority == PriorityLevel::kHigh) {
        query.bindValue(":priority", "High");
    } else if (sprint_backlog_item.priority == PriorityLevel::kMedium) {
        query.bindValue(":priority", "Medium");
    } else {
        query.bindValue(":priority", "Low");
    }

    if (sprint_backlog_item.status == TaskStatus::kToDo) {
        query.bindValue(":status", "To Do");
    } else if (sprint_backlog_item.status == TaskStatus::kInProgress) {
        query.bindValue(":status", "In Progress");
    } else if (sprint_backlog_item.status == TaskStatus::kInReview) {
        query.bindValue(":status", "In Review");
    } else {
        query.bindValue(":status", "Done");
    } 
}

void SprintBacklogItemMapper::bind_update(QSqlQuery &query, const SprintBacklogItem &sprint_backlog_item)
{
    query.bindValue(":id", QVariant::fromValue(sprint_backlog_item.id));
    query.bindValue(":product_backlog_item_id", QVariant::fromValue(sprint_backlog_item.product_backlog_item_id));
    query.bindValue(":sprint_id", QVariant::fromValue(sprint_backlog_item.sprint_id));

    query.bindValue(":name", QVariant::fromValue(sprint_backlog_item.name));
    query.bindValue(":description", QVariant::fromValue(sprint_backlog_item.description));

    query.bindValue(":story_points", QVariant::fromValue(sprint_backlog_item.story_points));    

    if (sprint_backlog_item.priority == PriorityLevel::kBlocker) {
        query.bindValue(":priority", "Blocker");
    } else if (sprint_backlog_item.priority == PriorityLevel::kHigh) {
        query.bindValue(":priority", "High");
    } else if (sprint_backlog_item.priority == PriorityLevel::kMedium) {
        query.bindValue(":priority", "Medium");
    } else {
        query.bindValue(":priority", "Low");
    }

    if (sprint_backlog_item.status == TaskStatus::kToDo) {
        query.bindValue(":status", "To Do");
    } else if (sprint_backlog_item.status == TaskStatus::kInProgress) {
        query.bindValue(":status", "In Progress");
    } else if (sprint_backlog_item.status == TaskStatus::kInReview) {
        query.bindValue(":status", "In Review");
    } else {
        query.bindValue(":status", "Done");
    } 
}

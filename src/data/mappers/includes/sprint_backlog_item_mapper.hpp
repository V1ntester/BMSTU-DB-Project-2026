#pragma once

#include <QSqlQuery>

#include "sprint_backlog_item.hpp"

class SprintBacklogItemMapper {
    public:
        static SprintBacklogItem from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const SprintBacklogItem &sprint_backlog_item);
        static void bind_update(QSqlQuery &query, const SprintBacklogItem &sprint_backlog_item);
};
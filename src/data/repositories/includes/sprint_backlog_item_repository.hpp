#pragma once

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "sprint_backlog_item.hpp"

class SprintBacklogItemRepository : Repository {
    public:
        explicit SprintBacklogItemRepository(QSqlDatabase db);
    
        void insert(const SprintBacklogItem &sprint_backlog_item);
        void update(const SprintBacklogItem &sprint_backlog_item);
        void remove(long id);

        std::optional<SprintBacklogItem> find_by_id(long id);
        std::vector<SprintBacklogItem> find_all();
};
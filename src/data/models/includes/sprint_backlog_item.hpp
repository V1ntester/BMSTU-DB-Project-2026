#pragma once

#include <QDateTime>
#include <QString>

enum class PriorityLevel {
    kBlocker,
    kHigh,
    kMedium,
    kLow,
};

enum class TaskStatus {
    kToDo,
    kInProgress,
    kInReview,
    kDone,
};

struct SprintBacklogItem {
    long id = 0;
    long product_backlog_item_id = 0;
    long sprint_id = 0;

    QString name;
    QString description;

    int story_points = 0;

    PriorityLevel priority = PriorityLevel::kLow;
    TaskStatus status = TaskStatus::kToDo;

    QDateTime created_at;
    QDateTime updated_at;
};
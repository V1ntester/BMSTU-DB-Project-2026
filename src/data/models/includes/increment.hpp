#pragma once

#include <QDateTime>
#include <QString>

struct Increment {
  long id = 0;
  long user_id = 0;
  long sprint_backlog_item_id = 0;

  QString note;

  int completed_story_points = 0;
  
  QDateTime created_at;
  QDateTime updated_at;
};
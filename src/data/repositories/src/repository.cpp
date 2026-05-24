#include "repository.hpp"

#include <QSqlDatabase>

Repository::Repository(QSqlDatabase db) : db_(db)
{
}

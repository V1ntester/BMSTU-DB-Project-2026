#pragma once 

#include <QSqlQuery>

#include "increment.hpp"

class IncrementMapper {
    public:
        static Increment from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const Increment &increment);
        static void bind_update(QSqlQuery &query, const Increment &increment);
};
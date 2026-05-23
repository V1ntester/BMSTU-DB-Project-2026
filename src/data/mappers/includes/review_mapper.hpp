#pragma once

#include <QSqlQuery>

#include "review.hpp"

class ReviewMapper {
    public:
        static Review from_query(const QSqlQuery &query);

        static void bind_insert(QSqlQuery &query, const Review &review);
        static void bind_update(QSqlQuery &query, const Review &review);
};
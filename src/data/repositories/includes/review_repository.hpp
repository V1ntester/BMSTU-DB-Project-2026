#pragma once

#include <QSqlDatabase>

#include <optional>
#include <vector>

#include "repository.hpp"
#include "review.hpp"

class ReviewRepository : public Repository {
    public:
        explicit ReviewRepository(QSqlDatabase db);

        void insert(const Review &review);
        void update(const Review &review);
        void remove(long id);

        std::optional<Review> find_by_id(long id);
        std::vector<Review> find_all();
};
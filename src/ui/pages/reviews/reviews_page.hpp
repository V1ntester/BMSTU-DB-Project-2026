#pragma once

#include <QWidget>
#include <optional>
#include <vector>

#include "review_repository.hpp"
#include "review.hpp"

#include "sprint_repository.hpp"

namespace Ui {
class ReviewsPage;
}

class ReviewsPage : public QWidget {
    Q_OBJECT

public:
    explicit ReviewsPage(QWidget* parent = nullptr);
    ~ReviewsPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void loadCombos();
    void fillTable();

    void fillForm(const Review& review);
    void clearForm();

    Review readForm() const;

private:
    Ui::ReviewsPage* ui_;

    ReviewRepository repo_;
    SprintRepository sprint_repo_;

    std::vector<Review> cache_;
    std::optional<long long> selected_id_;
};
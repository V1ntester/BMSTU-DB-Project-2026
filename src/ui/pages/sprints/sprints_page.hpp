#pragma once

#include <QWidget>
#include <optional>
#include <vector>

#include "sprint.hpp"
#include "sprint_repository.hpp"
#include "product_repository.hpp"

namespace Ui {
class SprintsPage;
}

class SprintsPage : public QWidget {
    Q_OBJECT

public:
    explicit SprintsPage(QWidget* parent = nullptr);
    ~SprintsPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void fillTable();
    void loadCombos();

    void fillForm(const Sprint& sprint);
    void clearForm();

    Sprint readForm() const;

private:
    Ui::SprintsPage* ui_;

    SprintRepository repo_;
    ProductRepository product_repo_;

    std::vector<Sprint> cache_;

    std::optional<long long> selected_id_;
};
#pragma once

#include <QWidget>
#include <optional>
#include <vector>

#include "sprint_backlog_item.hpp"
#include "sprint_backlog_item_repository.hpp"

#include "product_backlog_item_repository.hpp"
#include "sprint_repository.hpp"

namespace Ui {
class SprintBacklogItemsPage;
}

class SprintBacklogItemsPage : public QWidget {
    Q_OBJECT

public:
    explicit SprintBacklogItemsPage(QWidget* parent = nullptr);
    ~SprintBacklogItemsPage();

    void setSearchText(const QString& text);

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void loadCombos();
    void fillTable();
    void fillForm(const SprintBacklogItem& item);
    void clearForm();
    SprintBacklogItem readForm() const;

private:
    Ui::SprintBacklogItemsPage* ui_;

    SprintBacklogItemRepository repo_;
    ProductBacklogItemRepository product_repo_;
    SprintRepository sprint_repo_;

    std::vector<SprintBacklogItem> cache_;
    std::vector<SprintBacklogItem> view_;

    std::optional<long long> selected_id_;
};
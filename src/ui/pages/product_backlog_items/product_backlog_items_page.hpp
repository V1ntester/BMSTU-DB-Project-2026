#pragma once

#include <QWidget>
#include <optional>
#include <vector>

#include "product_backlog_item_repository.hpp"
#include "product_repository.hpp"

namespace Ui {
class ProductBacklogItemsPage;
}

class ProductBacklogItemsPage : public QWidget {
    Q_OBJECT

public:
    explicit ProductBacklogItemsPage(QWidget* parent = nullptr);
    ~ProductBacklogItemsPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void loadCombos();
    void fillTable();

    void fillForm(const ProductBacklogItem& item);
    void clearForm();

    ProductBacklogItem readForm() const;

private:
    Ui::ProductBacklogItemsPage* ui_;

    ProductBacklogItemRepository repo_;
    ProductRepository product_repo_;

    std::vector<ProductBacklogItem> cache_;
    std::optional<long long> selected_id_;
};
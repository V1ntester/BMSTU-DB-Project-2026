#pragma once

#include <QWidget>
#include <optional>
#include <vector>

#include "product_repository.hpp"
#include "product.hpp"

namespace Ui {
class ProductsPage;
}

class ProductsPage : public QWidget {
    Q_OBJECT

public:
    explicit ProductsPage(QWidget* parent = nullptr);
    ~ProductsPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void fillTable();

    void fillForm(const Product& product);
    void clearForm();

    Product readForm() const;

private:
    Ui::ProductsPage* ui_;
    ProductRepository repo_;

    std::vector<Product> cache_;
    std::optional<long long> selected_id_;
};
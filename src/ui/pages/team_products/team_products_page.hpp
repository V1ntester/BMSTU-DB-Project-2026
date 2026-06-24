#pragma once

#include <QWidget>

#include <optional>
#include <vector>

#include "team_product.hpp"
#include "team_product_repository.hpp"

#include "team_repository.hpp"
#include "product_repository.hpp"

namespace Ui {
class TeamProductsPage;
}

class TeamProductsPage : public QWidget {
    Q_OBJECT

public:
    explicit TeamProductsPage(QWidget* parent = nullptr);
    ~TeamProductsPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void loadCombos();

    void fillTable();

    void fillForm(
        const TeamProduct& tp
    );

    void clearForm();

    TeamProduct readForm() const;

private:
    Ui::TeamProductsPage* ui_;

    TeamProductRepository repo_;

    TeamRepository team_repo_;
    ProductRepository product_repo_;

    std::vector<TeamProduct> cache_;

    std::optional<long long> selected_team_id_;
    std::optional<long long> selected_product_id_;
};
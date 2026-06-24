#pragma once

#include <QWidget>

#include <optional>
#include <vector>

#include "role.hpp"
#include "role_repository.hpp"

namespace Ui {
class RolesPage;
}

class RolesPage : public QWidget {
    Q_OBJECT

public:
    explicit RolesPage(QWidget* parent = nullptr);
    ~RolesPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();

    void fillTable();

    void fillForm(const Role& role);
    void clearForm();

    Role readForm() const;

private:
    Ui::RolesPage* ui_;

    RoleRepository repo_;

    std::vector<Role> cache_;

    std::optional<long long> selected_id_;
};
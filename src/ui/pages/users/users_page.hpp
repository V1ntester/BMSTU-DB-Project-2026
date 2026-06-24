#pragma once

#include <QWidget>
#include <QDateTime>

#include <optional>
#include <vector>

#include "user.hpp"
#include "user_repository.hpp"

#include "role_repository.hpp"
#include "position_repository.hpp"
#include "team_repository.hpp"

class Ui_UsersPage;

class UsersPage : public QWidget {
    Q_OBJECT

public:
    explicit UsersPage(QWidget* parent = nullptr);
    ~UsersPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void loadCombos();

    void fillTable();

    void fillForm(const User& u);
    void clearForm();

    User readForm() const;

private:
    Ui_UsersPage* ui_;

    UserRepository repo_;

    RoleRepository role_repo_;
    PositionRepository position_repo_;
    TeamRepository team_repo_;

    std::vector<User> cache_;

    std::optional<long> selected_id_;
};
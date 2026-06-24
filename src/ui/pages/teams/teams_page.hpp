#pragma once

#include <QWidget>
#include <QDateTime>
#include <optional>
#include <vector>

#include "team.hpp"
#include "team_repository.hpp"

class Ui_TeamsPage;

class TeamsPage : public QWidget {
    Q_OBJECT

public:
    explicit TeamsPage(QWidget* parent = nullptr);
    ~TeamsPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void fillTable();
    void fillForm(const Team& team);
    void clearForm();
    Team readForm() const;

private:
    Ui_TeamsPage* ui_;
    TeamRepository repo_;

    std::vector<Team> cache_;
    std::optional<long> selected_id_;
};
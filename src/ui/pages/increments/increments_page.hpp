#pragma once

#include <QWidget>
#include <optional>
#include <vector>

#include "increment.hpp"
#include "increment_repository.hpp"
#include "user_repository.hpp"
#include "sprint_backlog_item_repository.hpp"

namespace Ui {
class IncrementsPage;
}

class IncrementsPage : public QWidget {
    Q_OBJECT

public:
    explicit IncrementsPage(QWidget* parent = nullptr);
    ~IncrementsPage();

signals:
    void sprintBacklogItemRequested(const QString& name);

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();
    void onPrintClicked();

private:
    void loadCache();
    void loadCombos();
    void fillTable();
    void fillForm(const Increment& inc);
    void clearForm();
    Increment readForm() const;

private:
    Ui::IncrementsPage* ui_;

    IncrementRepository repo_;
    UserRepository user_repo_;
    SprintBacklogItemRepository sprint_repo_;

    std::vector<Increment> cache_;
    std::vector<Increment> view_;

    std::optional<long long> selected_id_;
};
#pragma once

#include <QWidget>
#include <optional>
#include <vector>

#include "position_repository.hpp"
#include "position.hpp"

namespace Ui {
class PositionsPage;
}

class PositionsPage : public QWidget {
    Q_OBJECT

public:
    explicit PositionsPage(QWidget* parent = nullptr);
    ~PositionsPage();

private slots:
    void onAddClicked();
    void onSaveClicked();
    void onRemoveClicked();
    void onTableSelectionChanged();

private:
    void loadCache();
    void fillTable();

    void fillForm(const Position& position);
    void clearForm();

    Position readForm() const;

private:
    Ui::PositionsPage* ui_;
    PositionRepository repo_;

    std::vector<Position> cache_;
    std::optional<long long> selected_id_;
};
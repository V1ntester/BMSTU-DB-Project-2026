#include "positions_page.hpp"
#include "ui_positions_page.h"

#include <QSqlDatabase>
#include <QDebug>
#include <QAbstractItemView>
#include <QHeaderView>

PositionsPage::PositionsPage(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::PositionsPage),
      repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(ui_->addButton, &QPushButton::clicked,
            this, &PositionsPage::onAddClicked);

    connect(ui_->saveButton, &QPushButton::clicked,
            this, &PositionsPage::onSaveClicked);

    connect(ui_->removeButton, &QPushButton::clicked,
            this, &PositionsPage::onRemoveClicked);

    connect(ui_->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &PositionsPage::onTableSelectionChanged);

    loadCache();

    ui_->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

PositionsPage::~PositionsPage()
{
    delete ui_;
}

// ===================== LOAD =====================

void PositionsPage::loadCache()
{
    cache_ = repo_.find_all();

    fillTable();

    clearForm();
}

// ===================== TABLE =====================

void PositionsPage::fillTable()
{
    ui_->tableWidget->clear();

    ui_->tableWidget->setRowCount((int)cache_.size());
    ui_->tableWidget->setColumnCount(3);

    ui_->tableWidget->verticalHeader()->setVisible(false);

    ui_->tableWidget->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    ui_->tableWidget->setHorizontalHeaderLabels({
        "ИД",
        "Название",
        "Создан"
    });

    for (int i = 0; i < (int)cache_.size(); ++i) {

        const auto& p =
            cache_[i];

        ui_->tableWidget->setItem(
            i,
            0,
            new QTableWidgetItem(
                QString::number(
                    p.id)));

        ui_->tableWidget->setItem(
            i,
            1,
            new QTableWidgetItem(
                p.name));

        ui_->tableWidget->setItem(
            i,
            2,
            new QTableWidgetItem(
                p.created_at.toString(
                    "yyyy-MM-dd HH:mm")));
    }

    ui_->tableWidget
        ->resizeColumnsToContents();
}

// ===================== SELECTION =====================

void PositionsPage::onTableSelectionChanged()
{
    int row =
        ui_->tableWidget
            ->currentRow();

    if (row < 0 ||
        row >= (int)cache_.size())
        return;

    const auto& p =
        cache_[row];

    selected_id_ =
        p.id;

    fillForm(p);
}

// ===================== FORM =====================

void PositionsPage::fillForm(
    const Position& position)
{
    ui_->nameLineEdit
        ->setText(
            position.name);
}

void PositionsPage::clearForm()
{
    selected_id_.reset();

    ui_->tableWidget
        ->clearSelection();

    ui_->nameLineEdit
        ->clear();
}

// ===================== ADD =====================

void PositionsPage::onAddClicked()
{
    clearForm();
}

// ===================== READ FORM =====================

Position PositionsPage::readForm() const
{
    Position p;

    p.name =
        ui_->nameLineEdit
            ->text();

    if (selected_id_)
        p.id =
            *selected_id_;

    p.updated_at =
        QDateTime::currentDateTime();

    if (!selected_id_)
        p.created_at =
            p.updated_at;

    return p;
}

// ===================== SAVE =====================

void PositionsPage::onSaveClicked()
{
    try {

        auto p =
            readForm();

        if (selected_id_)
            repo_.update(p);
        else
            repo_.insert(p);

        loadCache();

    }
    catch (
        const std::exception& e)
    {
        qDebug()
            << "Save error:"
            << e.what();
    }
}

// ===================== DELETE =====================

void PositionsPage::onRemoveClicked()
{
    if (!selected_id_)
        return;

    try {

        repo_.remove(
            *selected_id_);

        loadCache();

    }
    catch (
        const std::exception& e)
    {
        qDebug()
            << "Delete error:"
            << e.what();
    }
}
#include "sprint_backlog_items_page.hpp"
#include "ui_sprint_backlog_items_page.h"

#include <QSqlDatabase>
#include <QVariant>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDebug>

// ===================== INIT =====================

SprintBacklogItemsPage::SprintBacklogItemsPage(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui::SprintBacklogItemsPage),
      repo_(QSqlDatabase::database()),
      product_repo_(QSqlDatabase::database()),
      sprint_repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(ui_->addButton, &QPushButton::clicked,
            this, &SprintBacklogItemsPage::onAddClicked);

    connect(ui_->saveButton, &QPushButton::clicked,
            this, &SprintBacklogItemsPage::onSaveClicked);

    connect(ui_->removeButton, &QPushButton::clicked,
            this, &SprintBacklogItemsPage::onRemoveClicked);

    connect(ui_->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &SprintBacklogItemsPage::onTableSelectionChanged);

    connect(ui_->searchLineEdit, &QLineEdit::textChanged,
            this, &SprintBacklogItemsPage::fillTable);

    ui_->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    loadCombos();
    loadCache();
}

SprintBacklogItemsPage::~SprintBacklogItemsPage()
{
    delete ui_;
}

// ===================== SEARCH =====================

void SprintBacklogItemsPage::setSearchText(const QString &text)
{
    ui_->searchLineEdit->setText(text);
    fillTable();
}

// ===================== COMBOS =====================

void SprintBacklogItemsPage::loadCombos()
{
    ui_->productBacklogItemComboBox->clear();
    ui_->sprintComboBox->clear();

    for (const auto &p : product_repo_.find_all())
    {
        ui_->productBacklogItemComboBox->addItem(
            p.name,
            QVariant::fromValue(static_cast<qlonglong>(p.id)));
    }

    for (const auto &s : sprint_repo_.find_all())
    {
        ui_->sprintComboBox->addItem(
            s.name,
            QVariant::fromValue(static_cast<qlonglong>(s.id)));
    }

    ui_->priorityComboBox->clear();
    ui_->priorityComboBox->addItem("Blocker", (int)PriorityLevel::kBlocker);
    ui_->priorityComboBox->addItem("High", (int)PriorityLevel::kHigh);
    ui_->priorityComboBox->addItem("Medium", (int)PriorityLevel::kMedium);
    ui_->priorityComboBox->addItem("Low", (int)PriorityLevel::kLow);

    ui_->statusComboBox->clear();
    ui_->statusComboBox->addItem("To Do", (int)TaskStatus::kToDo);
    ui_->statusComboBox->addItem("In Progress", (int)TaskStatus::kInProgress);
    ui_->statusComboBox->addItem("In Review", (int)TaskStatus::kInReview);
    ui_->statusComboBox->addItem("Done", (int)TaskStatus::kDone);
}

// ===================== CACHE =====================

void SprintBacklogItemsPage::loadCache()
{
    cache_ = repo_.find_all();
    fillTable();
    clearForm();
}

// ===================== TABLE =====================

void SprintBacklogItemsPage::fillTable()
{
    ui_->tableWidget->clear();

    QString filter = ui_->searchLineEdit->text().trimmed().toLower();

    view_.clear();

    for (const auto &item : cache_)
    {
        if (filter.isEmpty() ||
            item.name.toLower().contains(filter) ||
            item.product_backlog_item_name.toLower().contains(filter) ||
            item.sprint_name.toLower().contains(filter))
        {
            view_.push_back(item);
        }
    }

    ui_->tableWidget->setRowCount((int)view_.size());
    ui_->tableWidget->setColumnCount(8);

    ui_->tableWidget->setHorizontalHeaderLabels({"ИД",
                                                 "Название",
                                                 "Спринт",
                                                 "Продукт",
                                                 "Стори поинты",
                                                 "Приоритет",
                                                 "Статус",
                                                 "Создан"});

    for (int i = 0; i < (int)view_.size(); ++i)
    {
        const auto &item = view_[i];

        ui_->tableWidget->setItem(i, 0,
                                  new QTableWidgetItem(QString::number(item.id)));

        ui_->tableWidget->setItem(i, 1,
                                  new QTableWidgetItem(item.name));

        ui_->tableWidget->setItem(i, 2,
                                  new QTableWidgetItem(item.sprint_name));

        ui_->tableWidget->setItem(i, 3,
                                  new QTableWidgetItem(item.product_backlog_item_name));

        ui_->tableWidget->setItem(i, 4,
                                  new QTableWidgetItem(QString::number(item.story_points)));

        ui_->tableWidget->setItem(i, 5,
                                  new QTableWidgetItem(QString::number((int)item.priority)));

        ui_->tableWidget->setItem(i, 6,
                                  new QTableWidgetItem(QString::number((int)item.status)));

        ui_->tableWidget->setItem(
            i,
            7,
            new QTableWidgetItem(
                item.created_at
                    .toString(
                        "yyyy-MM-dd HH:mm")));
    }

    ui_->tableWidget->resizeColumnsToContents();
}

// ===================== SELECT =====================

void SprintBacklogItemsPage::onTableSelectionChanged()
{
    int row = ui_->tableWidget->currentRow();

    if (row < 0 || row >= (int)view_.size())
        return;

    selected_id_ = view_[row].id;
    fillForm(view_[row]);
}

// ===================== FORM =====================

void SprintBacklogItemsPage::fillForm(const SprintBacklogItem &item)
{
    ui_->nameLineEdit->setText(item.name);
    ui_->descriptionTextEdit->setPlainText(item.description);
    ui_->storyPointsLineEdit->setText(QString::number(item.story_points));

    ui_->productBacklogItemComboBox->setCurrentIndex(
        ui_->productBacklogItemComboBox->findData(
            QVariant::fromValue(static_cast<qlonglong>(item.product_backlog_item_id))));

    ui_->sprintComboBox->setCurrentIndex(
        ui_->sprintComboBox->findData(
            QVariant::fromValue(static_cast<qlonglong>(item.sprint_id))));

    ui_->priorityComboBox->setCurrentIndex((int)item.priority);
    ui_->statusComboBox->setCurrentIndex((int)item.status);
}

// ===================== CLEAR =====================

void SprintBacklogItemsPage::clearForm()
{
    selected_id_.reset();

    ui_->tableWidget->clearSelection();

    ui_->nameLineEdit->clear();
    ui_->descriptionTextEdit->clear();
    ui_->storyPointsLineEdit->clear();

    ui_->productBacklogItemComboBox->setCurrentIndex(-1);
    ui_->sprintComboBox->setCurrentIndex(-1);
}

// ===================== READ =====================

SprintBacklogItem SprintBacklogItemsPage::readForm() const
{
    SprintBacklogItem item;

    item.product_backlog_item_id =
        ui_->productBacklogItemComboBox->currentData().toLongLong();

    item.sprint_id =
        ui_->sprintComboBox->currentData().toLongLong();

    item.name = ui_->nameLineEdit->text();
    item.description = ui_->descriptionTextEdit->toPlainText();
    item.story_points = ui_->storyPointsLineEdit->text().toInt();

    item.priority = (PriorityLevel)ui_->priorityComboBox->currentData().toInt();
    item.status = (TaskStatus)ui_->statusComboBox->currentData().toInt();

    if (selected_id_)
        item.id = *selected_id_;

    return item;
}

// ===================== BUTTONS =====================

void SprintBacklogItemsPage::onAddClicked()
{
    clearForm();
}

void SprintBacklogItemsPage::onSaveClicked()
{
    auto item = readForm();

    if (selected_id_)
        repo_.update(item);
    else
        repo_.insert(item);

    loadCache();
}

void SprintBacklogItemsPage::onRemoveClicked()
{
    if (!selected_id_)
        return;

    repo_.remove(*selected_id_);
    loadCache();
}
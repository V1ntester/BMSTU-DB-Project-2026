#include "reviews_page.hpp"
#include "ui_reviews_page.h"

#include <QSqlDatabase>
#include <QVariant>
#include <QDebug>
#include <QHeaderView>
#include <QAbstractItemView>

// ===================== INIT =====================

ReviewsPage::ReviewsPage(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::ReviewsPage),
      repo_(QSqlDatabase::database()),
      sprint_repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(ui_->addButton, &QPushButton::clicked,
            this, &ReviewsPage::onAddClicked);

    connect(ui_->saveButton, &QPushButton::clicked,
            this, &ReviewsPage::onSaveClicked);

    connect(ui_->removeButton, &QPushButton::clicked,
            this, &ReviewsPage::onRemoveClicked);

    connect(ui_->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &ReviewsPage::onTableSelectionChanged);

    ui_->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    loadCombos();
    loadCache();
}

ReviewsPage::~ReviewsPage()
{
    delete ui_;
}

// ===================== COMBOS =====================

void ReviewsPage::loadCombos()
{
    ui_->sprintComboBox->clear();

    const auto sprints = sprint_repo_.find_all();

    for (const auto& sprint : sprints) {
        ui_->sprintComboBox->addItem(
            sprint.name,
            QVariant::fromValue(sprint.id)
        );
    }
}

// ===================== LOAD =====================

void ReviewsPage::loadCache()
{
    cache_ = repo_.find_all();
    fillTable();
    clearForm();
}

// ===================== TABLE =====================

void ReviewsPage::fillTable()
{
    ui_->tableWidget->clear();

    ui_->tableWidget->setRowCount((int)cache_.size());
    ui_->tableWidget->setColumnCount(5);

    ui_->tableWidget->verticalHeader()->setVisible(false);
    ui_->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui_->tableWidget->setHorizontalHeaderLabels({
        "ИД",
        "Спринт",
        "Название",
        "Описание",
        "Создан"
    });

    for (int i = 0; i < (int)cache_.size(); ++i) {
        const auto& review = cache_[i];

        ui_->tableWidget->setItem(i, 0,
            new QTableWidgetItem(QString::number(review.id)));

        ui_->tableWidget->setItem(i, 1,
            new QTableWidgetItem(review.sprint_name));

        ui_->tableWidget->setItem(i, 2,
            new QTableWidgetItem(review.name));

        ui_->tableWidget->setItem(i, 3,
            new QTableWidgetItem(review.description));

        ui_->tableWidget->setItem(i, 4,
            new QTableWidgetItem(
                review.created_at.toString("yyyy-MM-dd HH:mm")));
    }

    ui_->tableWidget->resizeColumnsToContents();
}

// ===================== SELECTION =====================

void ReviewsPage::onTableSelectionChanged()
{
    int row = ui_->tableWidget->currentRow();

    if (row < 0 || row >= (int)cache_.size())
        return;

    const auto& review = cache_[row];

    selected_id_ = review.id;
    fillForm(review);
}

// ===================== FORM =====================

void ReviewsPage::fillForm(const Review& review)
{
    ui_->nameLineEdit->setText(review.name);
    ui_->descriptionTextEdit->setPlainText(review.description);

    ui_->sprintComboBox->setCurrentIndex(
        ui_->sprintComboBox->findData(
            QVariant::fromValue(review.sprint_id)
        )
    );
}

// ===================== CLEAR =====================

void ReviewsPage::clearForm()
{
    selected_id_.reset();

    ui_->tableWidget->clearSelection();

    ui_->sprintComboBox->setCurrentIndex(-1);

    ui_->nameLineEdit->clear();
    ui_->descriptionTextEdit->clear();
}

// ===================== ADD =====================

void ReviewsPage::onAddClicked()
{
    clearForm();
}

// ===================== READ FORM =====================

Review ReviewsPage::readForm() const
{
    Review review;

    review.sprint_id =
        ui_->sprintComboBox->currentData().toLongLong();

    review.name =
        ui_->nameLineEdit->text();

    review.description =
        ui_->descriptionTextEdit->toPlainText();

    if (selected_id_)
        review.id = *selected_id_;

    review.updated_at = QDateTime::currentDateTime();

    if (!selected_id_)
        review.created_at = review.updated_at;

    return review;
}

// ===================== SAVE =====================

void ReviewsPage::onSaveClicked()
{
    try {
        auto review = readForm();

        if (selected_id_)
            repo_.update(review);
        else
            repo_.insert(review);

        loadCache();

    } catch (const std::exception& e) {
        qDebug() << "Save error:" << e.what();
    }
}

// ===================== DELETE =====================

void ReviewsPage::onRemoveClicked()
{
    if (!selected_id_)
        return;

    try {
        repo_.remove(*selected_id_);
        loadCache();

    } catch (const std::exception& e) {
        qDebug() << "Delete error:" << e.what();
    }
}
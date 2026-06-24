#include "product_backlog_items_page.hpp"
#include "ui_product_backlog_items_page.h"

#include <QSqlDatabase>
#include <QVariant>
#include <QDebug>
#include <QHeaderView>
#include <QAbstractItemView>

// ===================== INIT =====================

ProductBacklogItemsPage::ProductBacklogItemsPage(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::ProductBacklogItemsPage),
      repo_(QSqlDatabase::database()),
      product_repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(ui_->addButton, &QPushButton::clicked,
            this, &ProductBacklogItemsPage::onAddClicked);

    connect(ui_->saveButton, &QPushButton::clicked,
            this, &ProductBacklogItemsPage::onSaveClicked);

    connect(ui_->removeButton, &QPushButton::clicked,
            this, &ProductBacklogItemsPage::onRemoveClicked);

    connect(ui_->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &ProductBacklogItemsPage::onTableSelectionChanged);

    ui_->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    loadCombos();
    loadCache();
}

ProductBacklogItemsPage::~ProductBacklogItemsPage()
{
    delete ui_;
}

// ===================== COMBOS =====================

void ProductBacklogItemsPage::loadCombos()
{
    ui_->productComboBox->clear();

    const auto products = product_repo_.find_all();

    for (const auto& product : products) {
        ui_->productComboBox->addItem(
            product.name,
            QVariant::fromValue(product.id)
        );
    }
}

// ===================== LOAD =====================

void ProductBacklogItemsPage::loadCache()
{
    cache_ = repo_.find_all();
    fillTable();
    clearForm();
}

// ===================== TABLE =====================

void ProductBacklogItemsPage::fillTable()
{
    ui_->tableWidget->clear();

    ui_->tableWidget->setRowCount((int)cache_.size());
    ui_->tableWidget->setColumnCount(6);

    ui_->tableWidget->verticalHeader()->setVisible(false);
    ui_->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui_->tableWidget->setHorizontalHeaderLabels({
        "ИД",
        "Продукт",
        "Название",
        "Описание",
        "Стори поинты",
        "Создан"
    });

    for (int i = 0; i < (int)cache_.size(); ++i) {
        const auto& item = cache_[i];

        ui_->tableWidget->setItem(i, 0,
            new QTableWidgetItem(QString::number(item.id)));

        ui_->tableWidget->setItem(i, 1,
            new QTableWidgetItem(item.product_name));

        ui_->tableWidget->setItem(i, 2,
            new QTableWidgetItem(item.name));

        ui_->tableWidget->setItem(i, 3,
            new QTableWidgetItem(item.description));

        ui_->tableWidget->setItem(i, 4,
            new QTableWidgetItem(QString::number(item.story_points)));

        ui_->tableWidget->setItem(i, 5,
            new QTableWidgetItem(
                item.created_at.toString("yyyy-MM-dd HH:mm")));
    }

    ui_->tableWidget->resizeColumnsToContents();
}

// ===================== SELECTION =====================

void ProductBacklogItemsPage::onTableSelectionChanged()
{
    int row = ui_->tableWidget->currentRow();

    if (row < 0 || row >= (int)cache_.size())
        return;

    const auto& item = cache_[row];

    selected_id_ = item.id;

    fillForm(item);
}

// ===================== FORM =====================

void ProductBacklogItemsPage::fillForm(const ProductBacklogItem& item)
{
    ui_->nameLineEdit->setText(item.name);
    ui_->descriptionTextEdit->setPlainText(item.description);

    ui_->storyPointsLineEdit->setText(
        QString::number(item.story_points)
    );

    ui_->productComboBox->setCurrentIndex(
        ui_->productComboBox->findData(
            QVariant::fromValue(item.product_id)
        )
    );
}

// ===================== CLEAR =====================

void ProductBacklogItemsPage::clearForm()
{
    selected_id_.reset();

    ui_->tableWidget->clearSelection();

    ui_->productComboBox->setCurrentIndex(-1);

    ui_->nameLineEdit->clear();
    ui_->descriptionTextEdit->clear();
    ui_->storyPointsLineEdit->clear();
}

// ===================== ADD =====================

void ProductBacklogItemsPage::onAddClicked()
{
    clearForm();
}

// ===================== READ FORM =====================

ProductBacklogItem ProductBacklogItemsPage::readForm() const
{
    ProductBacklogItem item;

    item.product_id =
        ui_->productComboBox->currentData().toLongLong();

    item.name = ui_->nameLineEdit->text();
    item.description = ui_->descriptionTextEdit->toPlainText();
    item.story_points = ui_->storyPointsLineEdit->text().toInt();

    if (selected_id_)
        item.id = *selected_id_;

    item.updated_at = QDateTime::currentDateTime();

    if (!selected_id_)
        item.created_at = item.updated_at;

    return item;
}

// ===================== SAVE =====================

void ProductBacklogItemsPage::onSaveClicked()
{
    try {
        auto item = readForm();

        if (selected_id_)
            repo_.update(item);
        else
            repo_.insert(item);

        loadCache();

    } catch (const std::exception& e) {
        qDebug() << "Save error:" << e.what();
    }
}

// ===================== DELETE =====================

void ProductBacklogItemsPage::onRemoveClicked()
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
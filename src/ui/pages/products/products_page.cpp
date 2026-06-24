#include "products_page.hpp"
#include "ui_products_page.h"

#include <QSqlDatabase>
#include <QDebug>
#include <QAbstractItemView>
#include <QHeaderView>

ProductsPage::ProductsPage(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::ProductsPage),
      repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(
        ui_->addButton,
        &QPushButton::clicked,
        this,
        &ProductsPage::onAddClicked
    );

    connect(
        ui_->saveButton,
        &QPushButton::clicked,
        this,
        &ProductsPage::onSaveClicked
    );

    connect(
        ui_->removeButton,
        &QPushButton::clicked,
        this,
        &ProductsPage::onRemoveClicked
    );

    connect(
        ui_->tableWidget,
        &QTableWidget::itemSelectionChanged,
        this,
        &ProductsPage::onTableSelectionChanged
    );

    loadCache();

    ui_->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

ProductsPage::~ProductsPage()
{
    delete ui_;
}

// ===================== LOAD =====================

void ProductsPage::loadCache()
{
    cache_ =
        repo_.find_all();

    fillTable();

    clearForm();
}

// ===================== TABLE =====================

void ProductsPage::fillTable()
{
    ui_->tableWidget->clear();

    ui_->tableWidget
        ->setRowCount(
            (int)cache_.size()
        );

    ui_->tableWidget
        ->setColumnCount(4);

    ui_->tableWidget
        ->verticalHeader()
        ->setVisible(false);

    ui_->tableWidget
        ->setEditTriggers(
            QAbstractItemView::NoEditTriggers
        );

    ui_->tableWidget
        ->setHorizontalHeaderLabels({
            "ИД",
            "Название",
            "Описание",
            "Создан"
        });

    for (
        int i = 0;
        i < (int)cache_.size();
        ++i
    ) {

        const auto& p =
            cache_[i];

        ui_->tableWidget
            ->setItem(
                i,
                0,
                new QTableWidgetItem(
                    QString::number(
                        p.id
                    )
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                1,
                new QTableWidgetItem(
                    p.name
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                2,
                new QTableWidgetItem(
                    p.description
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                3,
                new QTableWidgetItem(
                    p.created_at
                        .toString(
                            "yyyy-MM-dd HH:mm"
                        )
                )
            );
    }

    ui_->tableWidget
        ->resizeColumnsToContents();
}

// ===================== SELECTION =====================

void ProductsPage::
onTableSelectionChanged()
{
    int row =
        ui_->tableWidget
            ->currentRow();

    if (
        row < 0 ||
        row >= (int)cache_.size()
    ) {
        return;
    }

    const auto& p =
        cache_[row];

    selected_id_ =
        p.id;

    fillForm(p);
}

// ===================== FORM =====================

void ProductsPage::
fillForm(
    const Product& product
)
{
    ui_->nameLineEdit
        ->setText(
            product.name
        );

    ui_->descriptionTextEdit
        ->setPlainText(
            product.description
        );
}

void ProductsPage::
clearForm()
{
    selected_id_.reset();

    ui_->tableWidget
        ->clearSelection();

    ui_->nameLineEdit
        ->clear();

    ui_->descriptionTextEdit
        ->clear();
}

// ===================== ADD =====================

void ProductsPage::
onAddClicked()
{
    clearForm();
}

// ===================== READ FORM =====================

Product
ProductsPage::
readForm() const
{
    Product p;

    p.name =
        ui_->nameLineEdit
            ->text();

    p.description =
        ui_->descriptionTextEdit
            ->toPlainText();

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

void ProductsPage::
onSaveClicked()
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
        const std::exception& e
    ) {

        qDebug()
            << "Save error:"
            << e.what();
    }
}

// ===================== DELETE =====================

void ProductsPage::
onRemoveClicked()
{
    if (!selected_id_)
        return;

    try {

        repo_.remove(
            *selected_id_
        );

        loadCache();

    }
    catch (
        const std::exception& e
    ) {

        qDebug()
            << "Delete error:"
            << e.what();
    }
}
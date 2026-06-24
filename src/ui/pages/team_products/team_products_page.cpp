#include "team_products_page.hpp"
#include "ui_team_products_page.h"

#include <QSqlDatabase>

#include <QVariant>
#include <QDebug>

#include <QHeaderView>
#include <QAbstractItemView>

// ================= INIT =================

TeamProductsPage::TeamProductsPage(
    QWidget* parent
)
    : QWidget(parent),
      ui_(new Ui::TeamProductsPage),
      repo_(QSqlDatabase::database()),
      team_repo_(QSqlDatabase::database()),
      product_repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(
        ui_->addButton,
        &QPushButton::clicked,
        this,
        &TeamProductsPage::onAddClicked
    );

    connect(
        ui_->saveButton,
        &QPushButton::clicked,
        this,
        &TeamProductsPage::onSaveClicked
    );

    connect(
        ui_->removeButton,
        &QPushButton::clicked,
        this,
        &TeamProductsPage::onRemoveClicked
    );

    connect(
        ui_->tableWidget,
        &QTableWidget::itemSelectionChanged,
        this,
        &TeamProductsPage::onTableSelectionChanged
    );

    loadCombos();
    loadCache();

    ui_->tableWidget->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    ui_->tableWidget->setSelectionMode(
        QAbstractItemView::SingleSelection
    );
}

TeamProductsPage::~TeamProductsPage()
{
    delete ui_;
}

// ================= COMBOS =================

void TeamProductsPage::loadCombos()
{
    ui_->teamComboBox->clear();
    ui_->productComboBox->clear();

    const auto teams =
        team_repo_.find_all();

    for (const auto& team : teams)
    {
        ui_->teamComboBox
            ->addItem(
                team.name,
                QVariant::fromValue(
                    team.id
                )
            );
    }

    const auto products =
        product_repo_.find_all();

    for (const auto& product : products)
    {
        ui_->productComboBox
            ->addItem(
                product.name,
                QVariant::fromValue(
                    product.id
                )
            );
    }

    ui_->teamComboBox
        ->setCurrentIndex(-1);

    ui_->productComboBox
        ->setCurrentIndex(-1);
}

// ================= LOAD =================

void TeamProductsPage::loadCache()
{
    cache_ =
        repo_.find_all();

    fillTable();

    clearForm();
}

// ================= TABLE =================

void TeamProductsPage::fillTable()
{
    ui_->tableWidget->clear();

    ui_->tableWidget
        ->setRowCount(
            cache_.size()
        );

    ui_->tableWidget
        ->setColumnCount(2);

    ui_->tableWidget
        ->verticalHeader()
        ->setVisible(false);

    ui_->tableWidget
        ->setEditTriggers(
            QAbstractItemView::NoEditTriggers
        );

    ui_->tableWidget
        ->setHorizontalHeaderLabels({
            "Команда",
            "Продукт"
        });

    for (
        int i = 0;
        i < cache_.size();
        ++i
    )
    {
        const auto& tp =
            cache_[i];

        ui_->tableWidget
            ->setItem(
                i,
                0,
                new QTableWidgetItem(
                    tp.team_name
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                1,
                new QTableWidgetItem(
                    tp.product_name
                )
            );
    }

    ui_->tableWidget
        ->resizeColumnsToContents();
}

// ================= SELECT =================

void TeamProductsPage::
onTableSelectionChanged()
{
    int row =
        ui_->tableWidget
            ->currentRow();

    if (
        row < 0 ||
        row >= cache_.size()
    )
        return;

    const auto& tp =
        cache_[row];

    selected_team_id_ =
        tp.team_id;

    selected_product_id_ =
        tp.product_id;

    fillForm(tp);
}

// ================= FORM =================

void TeamProductsPage::
fillForm(
    const TeamProduct& tp
)
{
    ui_->teamComboBox
        ->setCurrentIndex(
            ui_->teamComboBox
                ->findData(
                    QVariant::fromValue(
                        tp.team_id
                    )
                )
        );

    ui_->productComboBox
        ->setCurrentIndex(
            ui_->productComboBox
                ->findData(
                    QVariant::fromValue(
                        tp.product_id
                    )
                )
        );
}

void TeamProductsPage::
clearForm()
{
    selected_team_id_.reset();

    selected_product_id_.reset();

    ui_->tableWidget
        ->clearSelection();

    ui_->teamComboBox
        ->setCurrentIndex(-1);

    ui_->productComboBox
        ->setCurrentIndex(-1);
}

// ================= ADD =================

void TeamProductsPage::
onAddClicked()
{
    clearForm();
}

// ================= READ =================

TeamProduct
TeamProductsPage::
readForm() const
{
    TeamProduct tp;

    tp.team_id =
        ui_->teamComboBox
            ->currentData()
            .toLongLong();

    tp.product_id =
        ui_->productComboBox
            ->currentData()
            .toLongLong();

    tp.created_at =
        QDateTime::currentDateTime();

    return tp;
}

// ================= SAVE =================

void TeamProductsPage::
onSaveClicked()
{
    try
    {
        repo_.insert(
            readForm()
        );

        loadCache();
    }
    catch (
        const std::exception& e
    )
    {
        qDebug()
            << e.what();
    }
}

// ================= DELETE =================

void TeamProductsPage::
onRemoveClicked()
{
    if (
        !selected_team_id_ ||
        !selected_product_id_
    )
        return;

    try
    {
        repo_.remove(
            *selected_product_id_,
            *selected_team_id_
        );

        loadCache();
    }
    catch (
        const std::exception& e
    )
    {
        qDebug()
            << e.what();
    }
}
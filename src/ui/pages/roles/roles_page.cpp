#include "roles_page.hpp"
#include "ui_roles_page.h"

#include <QSqlDatabase>

#include <QDebug>
#include <QHeaderView>
#include <QAbstractItemView>

// ===================== INIT =====================

RolesPage::RolesPage(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::RolesPage),
      repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(
        ui_->addButton,
        &QPushButton::clicked,
        this,
        &RolesPage::onAddClicked
    );

    connect(
        ui_->saveButton,
        &QPushButton::clicked,
        this,
        &RolesPage::onSaveClicked
    );

    connect(
        ui_->removeButton,
        &QPushButton::clicked,
        this,
        &RolesPage::onRemoveClicked
    );

    connect(
        ui_->tableWidget,
        &QTableWidget::itemSelectionChanged,
        this,
        &RolesPage::onTableSelectionChanged
    );

    loadCache();

    ui_->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

RolesPage::~RolesPage()
{
    delete ui_;
}

// ===================== LOAD =====================

void RolesPage::loadCache()
{
    cache_ =
        repo_.find_all();

    fillTable();

    clearForm();
}

// ===================== TABLE =====================

void RolesPage::fillTable()
{
    ui_->tableWidget->clear();

    ui_->tableWidget
        ->setRowCount(
            (int)cache_.size()
        );

    ui_->tableWidget
        ->setColumnCount(3);

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
            "Создан"
        });

    for (
        int i = 0;
        i < (int)cache_.size();
        ++i
    ) {

        const auto& role =
            cache_[i];

        ui_->tableWidget
            ->setItem(
                i,
                0,
                new QTableWidgetItem(
                    QString::number(
                        role.id
                    )
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                1,
                new QTableWidgetItem(
                    role.name
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                2,
                new QTableWidgetItem(
                    role.created_at
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

void RolesPage::
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

    const auto& role =
        cache_[row];

    selected_id_ =
        role.id;

    fillForm(role);
}

// ===================== FORM =====================

void RolesPage::
fillForm(
    const Role& role
)
{
    ui_->nameLineEdit
        ->setText(
            role.name
        );
}

void RolesPage::
clearForm()
{
    selected_id_.reset();

    ui_->tableWidget
        ->clearSelection();

    ui_->nameLineEdit
        ->clear();
}

// ===================== ADD =====================

void RolesPage::
onAddClicked()
{
    clearForm();
}

// ===================== READ FORM =====================

Role RolesPage::
readForm() const
{
    Role role;

    role.name =
        ui_->nameLineEdit
            ->text();

    if (selected_id_)
        role.id =
            *selected_id_;

    role.updated_at =
        QDateTime::currentDateTime();

    if (!selected_id_)
        role.created_at =
            role.updated_at;

    return role;
}

// ===================== SAVE =====================

void RolesPage::
onSaveClicked()
{
    try {

        auto role =
            readForm();

        if (selected_id_)
            repo_.update(
                role
            );
        else
            repo_.insert(
                role
            );

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

void RolesPage::
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
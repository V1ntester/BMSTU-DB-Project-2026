#include "sprints_page.hpp"
#include "ui_sprints_page.h"

#include <QSqlDatabase>
#include <QVariant>
#include <QDebug>

#include <QHeaderView>
#include <QAbstractItemView>

// ===================== INIT =====================

SprintsPage::SprintsPage(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::SprintsPage),
      repo_(QSqlDatabase::database()),
      product_repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(
        ui_->addButton,
        &QPushButton::clicked,
        this,
        &SprintsPage::onAddClicked
    );

    connect(
        ui_->saveButton,
        &QPushButton::clicked,
        this,
        &SprintsPage::onSaveClicked
    );

    connect(
        ui_->removeButton,
        &QPushButton::clicked,
        this,
        &SprintsPage::onRemoveClicked
    );

    connect(
        ui_->tableWidget,
        &QTableWidget::itemSelectionChanged,
        this,
        &SprintsPage::onTableSelectionChanged
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

SprintsPage::~SprintsPage()
{
    delete ui_;
}

// ===================== COMBOS =====================

void SprintsPage::loadCombos()
{
    ui_->productComboBox->clear();

    const auto products =
        product_repo_.find_all();

    for (const auto& product : products) {

        ui_->productComboBox->addItem(
            product.name,
            QVariant::fromValue(
                product.id
            )
        );
    }

    ui_->productComboBox
        ->setCurrentIndex(-1);
}

// ===================== LOAD =====================

void SprintsPage::loadCache()
{
    cache_ =
        repo_.find_all();

    fillTable();

    clearForm();
}

// ===================== TABLE =====================

void SprintsPage::fillTable()
{
    ui_->tableWidget->clear();

    ui_->tableWidget
        ->setRowCount(
            (int)cache_.size()
        );

    ui_->tableWidget
        ->setColumnCount(6);

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
            "Продукт",
            "Название",
            "Начало",
            "Конец",
            "Создан"
        });

    for (
        int i = 0;
        i < (int)cache_.size();
        ++i
    ) {

        const auto& s =
            cache_[i];

        ui_->tableWidget
            ->setItem(
                i,
                0,
                new QTableWidgetItem(
                    QString::number(
                        s.id
                    )
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                1,
                new QTableWidgetItem(
                    s.product_name
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                2,
                new QTableWidgetItem(
                    s.name
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                3,
                new QTableWidgetItem(
                    s.start_time.toString(
                        "yyyy-MM-dd HH:mm"
                    )
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                4,
                new QTableWidgetItem(
                    s.end_time.toString(
                        "yyyy-MM-dd HH:mm"
                    )
                )
            );

        ui_->tableWidget
            ->setItem(
                i,
                5,
                new QTableWidgetItem(
                    s.created_at.toString(
                        "yyyy-MM-dd HH:mm"
                    )
                )
            );
    }

    ui_->tableWidget
        ->resizeColumnsToContents();
}

// ===================== SELECTION =====================

void SprintsPage::
onTableSelectionChanged()
{
    int row =
        ui_->tableWidget
            ->currentRow();

    if (
        row < 0 ||
        row >= (int)cache_.size()
    )
        return;

    selected_id_ =
        cache_[row].id;

    fillForm(
        cache_[row]
    );
}

// ===================== FORM =====================

void SprintsPage::
fillForm(
    const Sprint& s
)
{
    ui_->nameLineEdit
        ->setText(
            s.name
        );

    ui_->goalLineEdit
        ->setText(
            s.goal
        );

    ui_->startTimeEdit
        ->setDateTime(
            s.start_time
        );

    ui_->endTimeEdit
        ->setDateTime(
            s.end_time
        );

    ui_->productComboBox
        ->setCurrentIndex(
            ui_->productComboBox
                ->findData(
                    QVariant::fromValue(
                        s.product_id
                    )
                )
        );
}

// ===================== CLEAR =====================

void SprintsPage::
clearForm()
{
    selected_id_.reset();

    ui_->tableWidget
        ->clearSelection();

    ui_->productComboBox
        ->setCurrentIndex(-1);

    ui_->nameLineEdit
        ->clear();

    ui_->goalLineEdit
        ->clear();

    auto now =
        QDateTime::currentDateTime();

    ui_->startTimeEdit
        ->setDateTime(now);

    ui_->endTimeEdit
        ->setDateTime(now);
}

// ===================== READ =====================

Sprint
SprintsPage::
readForm() const
{
    Sprint s;

    s.product_id =
        ui_->productComboBox
            ->currentData()
            .toLongLong();

    s.name =
        ui_->nameLineEdit
            ->text();

    s.goal =
        ui_->goalLineEdit
            ->text();

    s.start_time =
        ui_->startTimeEdit
            ->dateTime();

    s.end_time =
        ui_->endTimeEdit
            ->dateTime();

    if (selected_id_)
        s.id =
            *selected_id_;

    s.updated_at =
        QDateTime::currentDateTime();

    if (!selected_id_)
        s.created_at =
            s.updated_at;

    return s;
}

// ===================== ADD =====================

void SprintsPage::
onAddClicked()
{
    clearForm();
}

// ===================== SAVE =====================

void SprintsPage::
onSaveClicked()
{
    try {

        auto s =
            readForm();

        if (selected_id_)
            repo_.update(s);
        else
            repo_.insert(s);

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

void SprintsPage::
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
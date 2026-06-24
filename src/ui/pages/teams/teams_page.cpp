#include "teams_page.hpp"
#include "ui_teams_page.h"

#include <QSqlDatabase>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDebug>

TeamsPage::TeamsPage(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui_TeamsPage),
      repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(ui_->addButton, &QPushButton::clicked,
            this, &TeamsPage::onAddClicked);

    connect(ui_->saveButton, &QPushButton::clicked,
            this, &TeamsPage::onSaveClicked);

    connect(ui_->removeButton, &QPushButton::clicked,
            this, &TeamsPage::onRemoveClicked);

    connect(ui_->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &TeamsPage::onTableSelectionChanged);

    loadCache();

    ui_->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

TeamsPage::~TeamsPage()
{
    delete ui_;
}

// ===================== ADD =====================

void TeamsPage::onAddClicked()
{
    clearForm();
}

// ===================== LOAD =====================

void TeamsPage::loadCache()
{
    cache_ = repo_.find_all();
    fillTable();

    clearForm();
}

// ===================== TABLE =====================

void TeamsPage::fillTable()
{
    ui_->tableWidget->clear();
    ui_->tableWidget->setRowCount((int)cache_.size());
    ui_->tableWidget->setColumnCount(3);

    ui_->tableWidget->setHorizontalHeaderLabels({
        "ID", "Название", "Создан"
    });

    ui_->tableWidget->verticalHeader()->setVisible(false);
    ui_->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    for (int i = 0; i < (int)cache_.size(); ++i) {

        const auto& t = cache_[i];

        ui_->tableWidget->setItem(i, 0,
            new QTableWidgetItem(QString::number(t.id)));

        ui_->tableWidget->setItem(i, 1,
            new QTableWidgetItem(t.name));

        ui_->tableWidget->setItem(i, 2,
            new QTableWidgetItem(
                t.created_at.toString("yyyy-MM-dd HH:mm")
            ));
    }

    ui_->tableWidget->resizeColumnsToContents();
}

// ===================== SELECT =====================

void TeamsPage::onTableSelectionChanged()
{
    int row = ui_->tableWidget->currentRow();

    if (row < 0 || row >= (int)cache_.size())
        return;

    selected_id_ = cache_[row].id;
    fillForm(cache_[row]);
}

// ===================== FORM =====================

void TeamsPage::fillForm(const Team& team)
{
    ui_->nameLineEdit->setText(team.name);
}

// ===================== CLEAR =====================

void TeamsPage::clearForm()
{
    selected_id_.reset();

    ui_->tableWidget->clearSelection();
    ui_->nameLineEdit->clear();
}

// ===================== READ =====================

Team TeamsPage::readForm() const
{
    Team t;

    t.name = ui_->nameLineEdit->text();

    if (selected_id_)
        t.id = *selected_id_;

    t.updated_at = QDateTime::currentDateTime();

    if (!selected_id_)
        t.created_at = t.updated_at;

    return t;
}

// ===================== SAVE =====================

void TeamsPage::onSaveClicked()
{
    try {
        auto t = readForm();

        if (selected_id_)
            repo_.update(t);
        else
            repo_.insert(t);

        loadCache();
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

// ===================== DELETE =====================

void TeamsPage::onRemoveClicked()
{
    if (!selected_id_)
        return;

    try {
        repo_.remove(*selected_id_);
        loadCache();
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
    }
}
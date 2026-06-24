#include "increments_page.hpp"
#include "ui_increments_page.h"

#include <QSqlDatabase>
#include <QVariant>
#include <QDebug>
#include <QHeaderView>
#include <QAbstractItemView>

#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDate>
#include <QMap>

// ===================== INIT =====================

IncrementsPage::IncrementsPage(QWidget* parent)
    : QWidget(parent),
      ui_(new Ui::IncrementsPage),
      repo_(QSqlDatabase::database()),
      user_repo_(QSqlDatabase::database()),
      sprint_repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(ui_->addButton, &QPushButton::clicked,
            this, &IncrementsPage::onAddClicked);

    connect(ui_->saveButton, &QPushButton::clicked,
            this, &IncrementsPage::onSaveClicked);

    connect(ui_->removeButton, &QPushButton::clicked,
            this, &IncrementsPage::onRemoveClicked);

    connect(ui_->printButton, &QPushButton::clicked,
            this, &IncrementsPage::onPrintClicked);

    connect(ui_->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &IncrementsPage::onTableSelectionChanged);

    connect(ui_->tableWidget, &QTableWidget::cellDoubleClicked,
            this, [this](int row, int) {
                if (row < 0 || row >= (int)view_.size())
                    return;

                emit sprintBacklogItemRequested(
                    view_[row].sprint_backlog_item_name
                );
            });

    connect(ui_->searchLineEdit, &QLineEdit::textChanged,
            this, &IncrementsPage::fillTable);

    ui_->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    loadCombos();
    loadCache();
}

IncrementsPage::~IncrementsPage()
{
    delete ui_;
}

// ===================== PRINT PDF =====================

void IncrementsPage::onPrintClicked()
{
    if (cache_.empty())
        return;

    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Сохранить отчет PDF",
        "increments_report.pdf",
        "PDF (*.pdf)"
    );

    if (fileName.isEmpty())
        return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);

    QMap<QString, QMap<int, int>> stats;

    for (const auto& inc : cache_) {
        int week = inc.created_at.date().weekNumber();
        stats[inc.user_email][week] += inc.completed_story_points;
    }

    int y = 120;

    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(100, y, "Отчет: Стори поинты по пользователям и неделям");
    y += 200;

    painter.setFont(QFont("Arial", 10));

    for (auto userIt = stats.begin(); userIt != stats.end(); ++userIt) {

        painter.setFont(QFont("Arial", 11, QFont::Bold));
        painter.drawText(100, y, userIt.key());
        y += 140;

        painter.setFont(QFont("Arial", 10));

        const auto& weeks = userIt.value();
        for (auto weekIt = weeks.begin(); weekIt != weeks.end(); ++weekIt) {

            QString line = QString("Неделя %1: %2 СП")
                               .arg(weekIt.key())
                               .arg(weekIt.value());

            painter.drawText(160, y, line);
            y += 110;

            if (y > 2700) {
                writer.newPage();
                y = 120;
            }
        }

        y += 120;
    }

    painter.end();
}

// ===================== COMBOS =====================

void IncrementsPage::loadCombos()
{
    ui_->userComboBox->clear();
    ui_->sprintBacklogItemComboBox->clear();

    for (const auto& user : user_repo_.find_all()) {
        ui_->userComboBox->addItem(
            user.email,
            QVariant::fromValue(static_cast<qlonglong>(user.id))
        );
    }

    for (const auto& item : sprint_repo_.find_all()) {
        ui_->sprintBacklogItemComboBox->addItem(
            item.name,
            QVariant::fromValue(static_cast<qlonglong>(item.id))
        );
    }
}

// ===================== CACHE =====================

void IncrementsPage::loadCache()
{
    cache_ = repo_.find_all();
    fillTable();
    clearForm();
}

// ===================== TABLE =====================

void IncrementsPage::fillTable()
{
    ui_->tableWidget->clear();

    QString filter = ui_->searchLineEdit->text().trimmed().toLower();

    view_.clear();

    for (const auto& inc : cache_) {
        if (filter.isEmpty() ||
            inc.user_email.toLower().contains(filter) ||
            inc.sprint_backlog_item_name.toLower().contains(filter))
        {
            view_.push_back(inc);
        }
    }

    ui_->tableWidget->setRowCount((int)view_.size());
    ui_->tableWidget->setColumnCount(5);

    ui_->tableWidget->setHorizontalHeaderLabels({
        "ИД",
        "Пользователь",
        "Элемент бэклога",
        "Стори поинты",
        "Создан"
    });

    for (int i = 0; i < (int)view_.size(); ++i) {
        const auto& inc = view_[i];

        ui_->tableWidget->setItem(i, 0,
            new QTableWidgetItem(QString::number(inc.id)));

        ui_->tableWidget->setItem(i, 1,
            new QTableWidgetItem(inc.user_email));

        ui_->tableWidget->setItem(i, 2,
            new QTableWidgetItem(inc.sprint_backlog_item_name));

        ui_->tableWidget->setItem(i, 3,
            new QTableWidgetItem(QString::number(inc.completed_story_points)));

        ui_->tableWidget->setItem(i, 4,
            new QTableWidgetItem(
                inc.created_at.toString("yyyy-MM-dd HH:mm")));
    }

    ui_->tableWidget->resizeColumnsToContents();
}

// ===================== SELECTION =====================

void IncrementsPage::onTableSelectionChanged()
{
    int row = ui_->tableWidget->currentRow();

    if (row < 0 || row >= (int)view_.size())
        return;

    selected_id_ = view_[row].id;
    fillForm(view_[row]);
}

// ===================== FORM =====================

void IncrementsPage::fillForm(const Increment& inc)
{
    ui_->noteTextEdit->setPlainText(inc.note);

    ui_->completedStoryPointsLineEdit->setText(
        QString::number(inc.completed_story_points));

    ui_->userComboBox->setCurrentIndex(
        ui_->userComboBox->findData(
            QVariant::fromValue(static_cast<qlonglong>(inc.user_id))
        )
    );

    ui_->sprintBacklogItemComboBox->setCurrentIndex(
        ui_->sprintBacklogItemComboBox->findData(
            QVariant::fromValue(static_cast<qlonglong>(inc.sprint_backlog_item_id))
        )
    );
}

// ===================== CLEAR =====================

void IncrementsPage::clearForm()
{
    selected_id_.reset();

    ui_->tableWidget->clearSelection();
    ui_->userComboBox->setCurrentIndex(-1);
    ui_->sprintBacklogItemComboBox->setCurrentIndex(-1);

    ui_->noteTextEdit->clear();
    ui_->completedStoryPointsLineEdit->clear();
}

// ===================== READ =====================

Increment IncrementsPage::readForm() const
{
    Increment inc;

    inc.user_id =
        ui_->userComboBox->currentData().toLongLong();

    inc.sprint_backlog_item_id =
        ui_->sprintBacklogItemComboBox->currentData().toLongLong();

    inc.note = ui_->noteTextEdit->toPlainText();
    inc.completed_story_points = ui_->completedStoryPointsLineEdit->text().toInt();

    if (selected_id_)
        inc.id = *selected_id_;

    inc.updated_at = QDateTime::currentDateTime();

    if (!selected_id_)
        inc.created_at = inc.updated_at;

    return inc;
}

// ===================== BUTTONS =====================

void IncrementsPage::onAddClicked()
{
    clearForm();
}

void IncrementsPage::onSaveClicked()
{
    try {
        auto inc = readForm();

        if (selected_id_)
            repo_.update(inc);
        else
            repo_.insert(inc);

        loadCache();
    }
    catch (const std::exception& e) {
        qDebug() << "Save error:" << e.what();
    }
}

void IncrementsPage::onRemoveClicked()
{
    if (!selected_id_)
        return;

    try {
        repo_.remove(*selected_id_);
        loadCache();
    }
    catch (const std::exception& e) {
        qDebug() << "Delete error:" << e.what();
    }
}
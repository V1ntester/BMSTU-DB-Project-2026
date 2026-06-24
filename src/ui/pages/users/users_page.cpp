#include "users_page.hpp"
#include "ui_users_page.h"

#include <QSqlDatabase>
#include <QVariant>

#include <QDebug>

#include <QHeaderView>
#include <QAbstractItemView>
#include <QRegularExpressionValidator>
#include <QMessageBox>

// ===================== INIT =====================

UsersPage::UsersPage(QWidget *parent)
    : QWidget(parent),
      ui_(new Ui_UsersPage),
      repo_(QSqlDatabase::database()),
      role_repo_(QSqlDatabase::database()),
      position_repo_(QSqlDatabase::database()),
      team_repo_(QSqlDatabase::database())
{
    ui_->setupUi(this);

    connect(
        ui_->addButton,
        &QPushButton::clicked,
        this,
        &UsersPage::onAddClicked);

    connect(
        ui_->saveButton,
        &QPushButton::clicked,
        this,
        &UsersPage::onSaveClicked);

    connect(
        ui_->removeButton,
        &QPushButton::clicked,
        this,
        &UsersPage::onRemoveClicked);

    connect(
        ui_->tableWidget,
        &QTableWidget::itemSelectionChanged,
        this,
        &UsersPage::onTableSelectionChanged);

    loadCombos();
    loadCache();

    ui_->tableWidget->setSelectionBehavior(
        QAbstractItemView::SelectRows);

    ui_->tableWidget->setSelectionMode(
        QAbstractItemView::SingleSelection);

    auto validator =
        new QRegularExpressionValidator(
            QRegularExpression(
                "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"),
            this);

    ui_->emailLineEdit
        ->setValidator(
            validator);
}

UsersPage::~UsersPage()
{
    delete ui_;
}

// ===================== LOAD =====================

void UsersPage::loadCache()
{
    cache_ =
        repo_.find_all();

    fillTable();

    clearForm();
}

// ===================== COMBOS =====================

void UsersPage::loadCombos()
{
    ui_->roleComboBox->clear();
    ui_->positionComboBox->clear();
    ui_->teamComboBox->clear();

    auto roles =
        role_repo_.find_all();

    for (const auto &role : roles)
    {

        ui_->roleComboBox
            ->addItem(
                role.name,
                QVariant::fromValue(
                    role.id));
    }

    auto positions =
        position_repo_.find_all();

    for (const auto &position : positions)
    {

        ui_->positionComboBox
            ->addItem(
                position.name,
                QVariant::fromValue(
                    position.id));
    }

    auto teams =
        team_repo_.find_all();

    for (const auto &team : teams)
    {

        ui_->teamComboBox
            ->addItem(
                team.name,
                QVariant::fromValue(
                    team.id));
    }

    ui_->roleComboBox->setCurrentIndex(-1);
    ui_->positionComboBox->setCurrentIndex(-1);
    ui_->teamComboBox->setCurrentIndex(-1);
}

// ===================== TABLE =====================

void UsersPage::fillTable()
{
    ui_->tableWidget->clear();

    ui_->tableWidget
        ->setRowCount(
            cache_.size());

    ui_->tableWidget
        ->setColumnCount(8);

    ui_->tableWidget
        ->verticalHeader()
        ->setVisible(false);

    ui_->tableWidget
        ->setEditTriggers(
            QAbstractItemView::NoEditTriggers);

    ui_->tableWidget
        ->setHorizontalHeaderLabels({"ИД",
                                     "Роль",
                                     "Должность",
                                     "Команда",
                                     "Имя",
                                     "Email",
                                     "Телефон",
                                     "Создан"});

    for (
        int i = 0;
        i < cache_.size();
        ++i)
    {

        const auto &u =
            cache_[i];

        ui_->tableWidget->setItem(
            i,
            0,
            new QTableWidgetItem(
                QString::number(
                    u.id)));

        ui_->tableWidget->setItem(
            i,
            1,
            new QTableWidgetItem(
                u.role_name));

        ui_->tableWidget->setItem(
            i,
            2,
            new QTableWidgetItem(
                u.position_name));

        ui_->tableWidget->setItem(
            i,
            3,
            new QTableWidgetItem(
                u.team_name));

        ui_->tableWidget->setItem(
            i,
            4,
            new QTableWidgetItem(
                u.first_name +
                " " +
                u.last_name));

        ui_->tableWidget->setItem(
            i,
            5,
            new QTableWidgetItem(
                u.email));

        ui_->tableWidget->setItem(
            i,
            6,
            new QTableWidgetItem(
                u.phone));

        ui_->tableWidget
            ->setItem(
                i,
                7,
                new QTableWidgetItem(u.created_at.toString("yyyy-MM-dd HH:mm")));
    }

    ui_->tableWidget
        ->resizeColumnsToContents();
}

// ===================== SELECT =====================

void UsersPage::onTableSelectionChanged()
{
    int row =
        ui_->tableWidget
            ->currentRow();

    if (
        row < 0 ||
        row >= cache_.size())
        return;

    selected_id_ =
        cache_[row].id;

    fillForm(
        cache_[row]);
}

// ===================== FORM =====================

void UsersPage::fillForm(
    const User &u)
{
    ui_->firstNameLineEdit->setText(u.first_name);
    ui_->lastNameLineEdit->setText(u.last_name);

    ui_->emailLineEdit->setText(u.email);
    ui_->phoneLineEdit->setText(u.phone);

    ui_->passwordHashLineEdit
        ->setText(
            u.password_hash);

    ui_->roleComboBox
        ->setCurrentIndex(
            ui_->roleComboBox
                ->findData(
                    QVariant::fromValue(
                        u.role_id)));

    ui_->positionComboBox
        ->setCurrentIndex(
            ui_->positionComboBox
                ->findData(
                    QVariant::fromValue(
                        u.position_id)));

    ui_->teamComboBox
        ->setCurrentIndex(
            ui_->teamComboBox
                ->findData(
                    QVariant::fromValue(
                        u.team_id)));
}

void UsersPage::clearForm()
{
    selected_id_.reset();

    ui_->tableWidget
        ->clearSelection();

    ui_->roleComboBox->setCurrentIndex(-1);
    ui_->positionComboBox->setCurrentIndex(-1);
    ui_->teamComboBox->setCurrentIndex(-1);

    ui_->firstNameLineEdit->clear();
    ui_->lastNameLineEdit->clear();

    ui_->emailLineEdit->clear();
    ui_->phoneLineEdit->clear();

    ui_->passwordHashLineEdit->clear();
}

// ===================== READ =====================

User UsersPage::readForm() const
{
    User u;

    u.role_id =
        ui_->roleComboBox
            ->currentData()
            .toLongLong();

    u.position_id =
        ui_->positionComboBox
            ->currentData()
            .toLongLong();

    u.team_id =
        ui_->teamComboBox
            ->currentData()
            .toLongLong();

    u.first_name =
        ui_->firstNameLineEdit->text();

    u.last_name =
        ui_->lastNameLineEdit->text();

    u.email =
        ui_->emailLineEdit->text();

    u.phone =
        ui_->phoneLineEdit->text();

    u.password_hash =
        ui_->passwordHashLineEdit->text();

    if (selected_id_)
        u.id =
            *selected_id_;

    u.updated_at =
        QDateTime::currentDateTime();

    if (!selected_id_)
        u.created_at =
            u.updated_at;

    return u;
}

// ===================== ACTIONS =====================

void UsersPage::onAddClicked()
{
    clearForm();
}

void UsersPage::onSaveClicked()
{
    if (
        !ui_->emailLineEdit
             ->hasAcceptableInput())
    {
        QMessageBox::warning(
            this,
            "Ошибка",
            "Введите корректный email");

        return;
    }

    auto user =
        readForm();

    if (selected_id_)
        repo_.update(user);
    else
        repo_.insert(user);

    loadCache();
}

void UsersPage::onRemoveClicked()
{
    if (!selected_id_)
        return;

    repo_.remove(
        *selected_id_);

    loadCache();
}
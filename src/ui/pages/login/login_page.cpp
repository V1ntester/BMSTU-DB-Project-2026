#include "login_page.hpp"
#include "ui_login_page.h"

#include <QMessageBox>
#include <QSqlDatabase>

LoginPage::LoginPage(
    QWidget* parent)
    :
    QWidget(parent),
    ui_(new Ui::LoginPage),
    repo_(
        QSqlDatabase::database()
    )
{
    ui_->setupUi(this);

    connect(
        ui_->loginButton,
        &QPushButton::clicked,
        this,
        &LoginPage::onLoginClicked
    );
}

void LoginPage::onLoginClicked()
{
    try {

        QString email =
            ui_->emailLineEdit
                ->text()
                .trimmed();

        QString password =
            ui_->passwordLineEdit
                ->text();

        auto user =
            repo_.find_by_email(
                email
            );

        if (!user) {

            QMessageBox::warning(
                this,
                "Ошибка",
                "Пользователь не найден"
            );

            return;
        }

        if (
            user->password_hash
            != password
        ) {
            QMessageBox::warning(
                this,
                "Ошибка",
                "Неверный пароль"
            );

            return;
        }

        emit logged_in(
            *user
        );
    }
    catch (
        const std::exception& e
    ) {
        QMessageBox::critical(
            this,
            "Ошибка",
            e.what()
        );
    }
}
#pragma once

#include <QWidget>

#include "user_repository.hpp"

namespace Ui {
class LoginPage;
}

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(
        QWidget* parent = nullptr
    );

signals:
    void logged_in(
        User user
    );

private slots:
    void onLoginClicked();

private:
    Ui::LoginPage* ui_;

    UserRepository repo_;
};
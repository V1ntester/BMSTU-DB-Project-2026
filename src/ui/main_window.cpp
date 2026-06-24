#include "main_window.hpp"

#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    auto* central = new QWidget;
    auto* root = new QVBoxLayout(central);

    auto* nav = new QWidget;
    navigation_ = new QHBoxLayout(nav);

    stack_ = new QStackedWidget;

    root->addWidget(nav);
    root->addWidget(stack_);

    setCentralWidget(central);
    resize(1366, 768);

    nav->setStyleSheet(R"(
        QWidget {
            background-color: #FFFFFF;
            border-bottom: 1px solid #D9DEE5;
            border-left: 1px solid #D9DEE5;
            border-right: 1px solid #D9DEE5;
            border-top: 1px solid #D9DEE5;
            padding: 6px;
        }

        QPushButton {
            background: transparent;
            color: #2C2F36;
            border: none;
            padding: 10px 14px;
            border-radius: 6px;
            font-weight: 500;
        }

        QPushButton:hover {
            background: #E6EBF2;
        }

        QPushButton:pressed {
            background: #DCE3EC;
        }

        QPushButton:checked {
            background: #7BC6FF;
        }
    )");
}

void MainWindow::register_page(
    QWidget* page,
    const QString& title_key,
    const QStringList& roles)
{
    stack_->addWidget(page);

    auto* button = new QPushButton(tr(qPrintable(title_key)));

    button->setCheckable(true);

    navigation_->addWidget(button);

    pages_[button] = { page, roles, title_key };

    connect(button, &QPushButton::clicked, this, [=] {
        set_page(page);
    });
}

void MainWindow::set_page(QWidget* page)
{
    stack_->setCurrentWidget(page);

    for (auto it = pages_.begin(); it != pages_.end(); ++it) {

        bool active = (it.value().page == page);

        it.key()->setChecked(active);
    }
}

void MainWindow::authorize(const User& user)
{
    QWidget* first = nullptr;

    for (auto it = pages_.begin(); it != pages_.end(); ++it) {

        bool allowed = it.value().roles.contains(user.role_name);

        it.key()->setVisible(allowed);

        if (allowed && !first) {
            first = it.value().page;
        }
    }

    if (first) {
        set_page(first);
    }
}

void MainWindow::retranslate_ui()
{
    for (auto it = pages_.begin(); it != pages_.end(); ++it) {
        it.key()->setText(tr(qPrintable(it.value().title_key)));
    }
}
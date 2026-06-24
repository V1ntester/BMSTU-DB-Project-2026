#pragma once

#include <QMainWindow>
#include <QMap>
#include <QStringList>

#include "user.hpp"

class QWidget;
class QPushButton;
class QStackedWidget;
class QHBoxLayout;

struct PageInfo {
    QWidget* page;
    QStringList roles;
    QString title_key;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    void register_page(
        QWidget* page,
        const QString& title_key,
        const QStringList& roles
    );

    void set_page(QWidget* page);

    void authorize(const User& user);

    void retranslate_ui();

private:
    QStackedWidget* stack_;
    QHBoxLayout* navigation_;

    QMap<QPushButton*, PageInfo> pages_;
};
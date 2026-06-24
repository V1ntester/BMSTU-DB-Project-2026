#include <QApplication>

#include "database_manager.hpp"

#include "ui/pages/login/login_page.hpp"
#include "ui/main_window.hpp"

#include "ui/pages/increments/increments_page.hpp"
#include "ui/pages/positions/positions_page.hpp"
#include "ui/pages/product_backlog_items/product_backlog_items_page.hpp"
#include "ui/pages/products/products_page.hpp"
#include "ui/pages/reviews/reviews_page.hpp"
#include "ui/pages/roles/roles_page.hpp"
#include "ui/pages/sprint_backlog_items/sprint_backlog_items_page.hpp"
#include "ui/pages/sprints/sprints_page.hpp"
#include "ui/pages/team_products/team_products_page.hpp"
#include "ui/pages/teams/teams_page.hpp"
#include "ui/pages/users/users_page.hpp"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    DatabaseManager db;

    if (!db.connect(
        "_",
        5432,
        "_",
        "_",
        "_"
    )) {
        return -1;
    }

    auto* login = new LoginPage;
    auto* window = new MainWindow;

    login->setWindowTitle("АИС Управление Проектами");
    window->setWindowTitle("АИС Управление Проектами");

    window->register_page(
        new RolesPage,
        "Роли",
        { "Администратор" }
    );

    window->register_page(
        new PositionsPage,
        "Должности",
        { "Администратор" }
    );

    window->register_page(
        new UsersPage,
        "Пользователи",
        { "Руководитель команды", "Продуктовый менеджер", "Администратор" }
    );

    window->register_page(
        new TeamsPage,
        "Команды",
        { "Руководитель команды", "Продуктовый менеджер", "Администратор" }
    );

    window->register_page(
        new TeamProductsPage,
        "Продукты команд",
        { "Продуктовый менеджер", "Администратор" }
    );

    window->register_page(
        new ProductsPage,
        "Продукты",
        { "Руководитель команды", "Продуктовый менеджер", "Администратор" }
    );

    window->register_page(
        new ProductBacklogItemsPage,
        "Бэклог продуктов",
        { "Руководитель команды", "Продуктовый менеджер", "Администратор" }
    );

    window->register_page(
        new SprintsPage,
        "Спринты",
        { "Разработчик", "Руководитель команды", "Продуктовый менеджер", "Администратор" }
    );

    window->register_page(
        new SprintBacklogItemsPage,
        "Бэклог спринтов",
        { "Разработчик", "Руководитель команды", "Продуктовый менеджер", "Администратор" }
    );

    window->register_page(
        new IncrementsPage,
        "Инкременты",
        { "Разработчик", "Руководитель команды", "Продуктовый менеджер", "Администратор" }
    );

    window->register_page(
        new ReviewsPage,
        "Ревью",
        { "Разработчик", "Руководитель команды", "Продуктовый менеджер", "Администратор" }
    );

    QObject::connect(
        login,
        &LoginPage::logged_in,
        [=](User user)
        {
            window->authorize(user);
            window->show();
            login->close();
        }
    );

    login->show();

    return a.exec();
}
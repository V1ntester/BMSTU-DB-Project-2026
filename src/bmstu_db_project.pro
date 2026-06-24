QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

INCLUDEPATH += $$PWD/data/db/includes \
               $$PWD/data/mappers/includes \
               $$PWD/data/models/includes \
               $$PWD/data/repositories/includes

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    data/db/src/database_manager.cpp \
    data/repositories/src/repository.cpp \
    data/mappers/src/increment_mapper.cpp \
    data/mappers/src/position_mapper.cpp \ 
    data/mappers/src/product_backlog_item_mapper.cpp \
    data/mappers/src/product_mapper.cpp \
    data/mappers/src/review_mapper.cpp \
    data/mappers/src/role_mapper.cpp \
    data/mappers/src/sprint_backlog_item_mapper.cpp \
    data/mappers/src/sprint_mapper.cpp \
    data/mappers/src/team_product_mapper.cpp \
    data/mappers/src/team_mapper.cpp \
    data/mappers/src/user_mapper.cpp \
    data/repositories/src/increment_repository.cpp \
    data/repositories/src/position_repository.cpp \
    data/repositories/src/product_backlog_item_repository.cpp \
    data/repositories/src/product_repository.cpp \
    data/repositories/src/review_repository.cpp \
    data/repositories/src/role_repository.cpp \
    data/repositories/src/sprint_backlog_item_repository.cpp \
    data/repositories/src/sprint_repository.cpp \
    data/repositories/src/team_product_repository.cpp \
    data/repositories/src/team_repository.cpp \
    data/repositories/src/user_repository.cpp \
    ui/main_window.cpp \ 
    ui/pages/increments/increments_page.cpp \
    ui/pages/login/login_page.cpp \
    ui/pages/positions/positions_page.cpp \
    ui/pages/product_backlog_items/product_backlog_items_page.cpp \
    ui/pages/products/products_page.cpp \
    ui/pages/reviews/reviews_page.cpp \
    ui/pages/roles/roles_page.cpp \
    ui/pages/sprint_backlog_items/sprint_backlog_items_page.cpp \
    ui/pages/sprints/sprints_page.cpp \
    ui/pages/team_products/team_products_page.cpp \ 
    ui/pages/teams/teams_page.cpp \
    ui/pages/users/users_page.cpp

HEADERS += \
    data/db/includes/database_manager.hpp \
    data/repositories/includes/repository.hpp \
    data/models/includes/increment.hpp \
    data/models/includes/position.hpp \
    data/models/includes/product_backlog_item.hpp \
    data/models/includes/product.hpp \
    data/models/includes/review.hpp \ 
    data/models/includes/role.hpp \
    data/models/includes/sprint_backlog_item.hpp \
    data/models/includes/sprint.hpp \
    data/models/includes/team_product.hpp \
    data/models/includes/team.hpp \
    data/models/includes/user.hpp \
    data/mappers/includes/increment_mapper.hpp \
    data/mappers/includes/position_mapper.hpp \
    data/mappers/includes/product_backlog_item_mapper.hpp \
    data/mappers/includes/product_mapper.hpp \
    data/mappers/includes/review_mapper.hpp \
    data/mappers/includes/role_mapper.hpp \
    data/mappers/includes/sprint_backlog_item_mapper.hpp \
    data/mappers/includes/sprint_mapper.hpp \
    data/mappers/includes/team_product_mapper.hpp \
    data/mappers/includes/team_mapper.hpp \ 
    data/mappers/includes/user_mapper.hpp \
    data/repositories/includes/increment_repository.hpp \ 
    data/repositories/includes/position_repository.hpp \ 
    data/repositories/includes/product_backlog_item_repository.hpp \
    data/repositories/includes/product_repository.hpp \ 
    data/repositories/includes/review_repository.hpp \
    data/repositories/includes/role_repository.hpp \
    data/repositories/includes/sprint_backlog_item_repository.hpp \
    data/repositories/includes/sprint_repository.hpp \
    data/repositories/includes/team_product_repository.hpp \
    data/repositories/includes/team_repository.hpp \
    data/repositories/includes/user_repository.hpp \
    ui/main_window.hpp \ 
    ui/pages/increments/increments_page.hpp \
    ui/pages/login/login_page.hpp \
    ui/pages/positions/positions_page.hpp \
    ui/pages/product_backlog_items/product_backlog_items_page.hpp \
    ui/pages/products/products_page.hpp \
    ui/pages/reviews/reviews_page.hpp \
    ui/pages/roles/roles_page.hpp \
    ui/pages/sprint_backlog_items/sprint_backlog_items_page.hpp \
    ui/pages/sprints/sprints_page.hpp \ 
    ui/pages/team_products/team_products_page.hpp \
    ui/pages/teams/teams_page.hpp \
    ui/pages/users/users_page.hpp

FORMS += \
    ui/pages/increments/increments_page.ui \
    ui/pages/login/login_page.ui \
    ui/pages/positions/positions_page.ui \
    ui/pages/products/products_page.ui \ 
    ui/pages/product_backlog_items/product_backlog_items_page.ui \ 
    ui/pages/reviews/reviews_page.ui \ 
    ui/pages/roles/roles_page.ui \
    ui/pages/sprint_backlog_items/sprint_backlog_items_page.ui \
    ui/pages/sprints/sprints_page.ui \
    ui/pages/team_products/team_products_page.ui \
    ui/pages/teams/teams_page.ui \ 
    ui/pages/users/users_page.ui 

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

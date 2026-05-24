#pragma once

#include <QSqlDatabase>

class Repository {
    public:
        explicit Repository(QSqlDatabase db);

    protected:
        QSqlDatabase db_;
};

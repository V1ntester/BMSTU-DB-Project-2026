#include "position_repository.hpp"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include <optional>
#include <vector>

#include "position.hpp"
#include "position_mapper.hpp"
#include "repository.hpp"

PositionRepository::PositionRepository(QSqlDatabase db) : Repository(db)
{
}

void PositionRepository::insert(const Position &position)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        INSERT INTO positions (
            name
        )
        VALUES (
            :name
        )
    )");

    PositionMapper::bind_insert(query, position);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void PositionRepository::update(const Position &position)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        UPDATE positions
        SET
            name = :name
        WHERE id = :id
    )");

    PositionMapper::bind_update(query, position);

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

void PositionRepository::remove(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        DELETE FROM positions
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }
}

std::optional<Position> PositionRepository::find_by_id(long id)
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM positions
        WHERE id = :id
    )");

    query.bindValue(":id", QVariant::fromValue(id));

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    if (!query.next()) {
        return std::nullopt;
    }

    return PositionMapper::from_query(query);
}

std::vector<Position> PositionRepository::find_all()
{
    QSqlQuery query(db_);

    query.prepare(R"(
        SELECT *
        FROM positions
    )");

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    std::vector<Position> positions;

    while (query.next()) {
        positions.push_back(PositionMapper::from_query(query));
    }

    return positions;
}

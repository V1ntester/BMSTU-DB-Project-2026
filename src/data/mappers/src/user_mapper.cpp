#include "user_mapper.hpp"

#include <QSqlQuery>
#include <QVariant>

#include "user.hpp"

User UserMapper::from_query(const QSqlQuery &query)
{
    User user;

    user.id = query.value("id").toLongLong();

    user.role_id = query.value("role_id").toLongLong();
    user.role_name = query.value("role_name").toString();

    user.position_id = query.value("position_id").toLongLong();
    user.position_name = query.value("position_name").toString();
    
    user.team_id = query.value("team_id").toLongLong();
    user.team_name = query.value("team_name").toString();

    user.first_name = query.value("first_name").toString();
    user.last_name = query.value("last_name").toString();

    user.email = query.value("email").toString();
    user.phone = query.value("phone").toString();

    user.password_hash = query.value("password_hash").toString();

    user.created_at = query.value("created_at").toDateTime();
    user.updated_at = query.value("updated_at").toDateTime();

    return user;
}

void UserMapper::bind_insert(QSqlQuery &query, const User &user)
{
    query.bindValue(":role_id", QVariant::fromValue(user.role_id));    
    query.bindValue(":position_id", QVariant::fromValue(user.position_id));
    query.bindValue(":team_id", QVariant::fromValue(user.team_id));

    query.bindValue(":first_name", QVariant::fromValue(user.first_name));
    query.bindValue(":last_name", QVariant::fromValue(user.last_name));

    query.bindValue(":email", QVariant::fromValue(user.email));
    query.bindValue(":phone", QVariant::fromValue(user.phone));    

    query.bindValue(":password_hash", QVariant::fromValue(user.password_hash));    
}

void UserMapper::bind_update(QSqlQuery &query, const User &user)
{
    query.bindValue(":id", QVariant::fromValue(user.id));    
    query.bindValue(":role_id", QVariant::fromValue(user.role_id));    
    query.bindValue(":position_id", QVariant::fromValue(user.position_id));
    query.bindValue(":team_id", QVariant::fromValue(user.team_id));

    query.bindValue(":first_name", QVariant::fromValue(user.first_name));
    query.bindValue(":last_name", QVariant::fromValue(user.last_name));

    query.bindValue(":email", QVariant::fromValue(user.email));
    query.bindValue(":phone", QVariant::fromValue(user.phone));    
    
    query.bindValue(":password_hash", QVariant::fromValue(user.password_hash)); 
}

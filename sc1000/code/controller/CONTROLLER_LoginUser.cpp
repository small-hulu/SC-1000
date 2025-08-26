#include "CONTROLLER_LoginUser.h"

namespace CONTROLLER {

namespace {
DB::TableEntity::user table;
}

int LoginUser::Get_allCount() {
    return DB::SqlExe::Get_all_count(table);
}

QList<ENTITY::User> LoginUser::Get_rangeEntity(int start, int end) {
    auto matrix = DB::SqlExe::Get_between(table, start, end);

    QList<ENTITY::User> res;
    ENTITY::User        log;
    for (auto &&arr : matrix) {
        log.Set_Values_WithId(arr);
        res << log;
    }
    return res;
}

bool LoginUser::Create_UserToDB(ENTITY::User &user) {
    auto id = DB::SqlExe::Insert_oneLine(user.cdbtable, user.Get_allValueList());
    user.Set_id(id);
    return user.Is_valid();
}

LoginUser::LoginUser() {
    using entity_attr = DB::TableEntity::user::table_attribute;
    DB::TableEntity::user dbtable;
    auto                  tableAllVals = DB::SqlExe::Get_all_value(dbtable);
    if (tableAllVals.size()) {
        return;
    }

    const QString time = QDateTime::currentDateTime().toString(user.time_format);
    ENTITY::User  user;

    user.table[entity_attr::username]   = "root";
    user.table[entity_attr::password]   = "root";
    user.table[entity_attr::level]      = QString::number(ENTITY::User::UserLevel::root);
    user.table[entity_attr::createtime] = time;
    DB::SqlExe::Insert_oneLine(dbtable, user.Get_allValueList());

    user.table[entity_attr::username]   = "admin";
    user.table[entity_attr::password]   = "admin";
    user.table[entity_attr::level]      = QString::number(ENTITY::User::UserLevel::admin);
    user.table[entity_attr::createtime] = time;
    DB::SqlExe::Insert_oneLine(dbtable, user.Get_allValueList());

    user.table[entity_attr::username]   = "user";
    user.table[entity_attr::password]   = "user";
    user.table[entity_attr::level]      = QString::number(ENTITY::User::UserLevel::user);
    user.table[entity_attr::createtime] = time;
    DB::SqlExe::Insert_oneLine(dbtable, user.Get_allValueList());
}

bool LoginUser::Try_login(const QString &name, const QString &password) {
    using entity_attr = DB::TableEntity::user::table_attribute;
    DB::TableEntity::user dbtable;

    auto tableAllVals = DB::SqlExe::Get_all_value(dbtable);
    for (auto &&arr : tableAllVals) {
        auto id = arr[0].toInt();
        arr.pop_front();
        assert(arr.size() == entity_attr::Enum_Count);
        const QString dbname     = arr[entity_attr::username];
        const QString dbpassword = arr[entity_attr::password];
        if (name == dbname && password == dbpassword) {
            user.Set_id(id);
            for (int i = 0; i < entity_attr::Enum_Count; i += 1) {
                user.table[(entity_attr)i] = arr[i];
            }
            return true;
        }
    }

    return false;
}
}  // namespace CONTROLLER

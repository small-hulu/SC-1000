#include "CONTROLLER_Log.h"

namespace CONTROLLER {

namespace {
DB::TableEntity::log table;
}

int Log::Get_allCount() {
    return DB::SqlExe::Get_all_count(table);
}

int Log::Get_allCount_byType(int type) {
    if (type < 0) {
        return Get_allCount();
    }

    auto query = QString(" SELECT count(*) FROM %1 where type = '%2' ");
    query      = query.arg(table.Get_tableName());
    query      = query.arg(ENTITY::Log::Get_logTypeString(type));

    auto res = DB::SqlExe::Get_ByOutSql(query);
    if (res.size() && res[0].size()) {
        return res[0][0].toInt();
    }
    return 0;
}

QList<ENTITY::Log> Log::Get_rangeEntity(int start, int end) {
    auto matrix = DB::SqlExe::Get_between(table, start, end);

    QList<ENTITY::Log> res;
    ENTITY::Log        log;
    for (auto&& arr : matrix) {
        log.Set_Values_WithId(arr);
        res << log;
    }
    return res;
}

QList<ENTITY::Log> Log::Get_rangeEntity_byType(int start, int end, int type) {
    if (type < 0) {
        return Get_rangeEntity(start, end);
    }

    auto query =
        QString("SELECT * FROM %1 where type = '%2' order by id desc limit %3 offset %4; ");
    query            = query.arg(table.Get_tableName());
    query            = query.arg(ENTITY::Log::Get_logTypeString(type));
    const int limit  = end - start + 1;
    const int offset = start - 1;
    query            = query.arg(limit).arg(offset);

    auto               matrix = DB::SqlExe::Get_ByOutSql(query);
    QList<ENTITY::Log> res;
    ENTITY::Log        log;
    for (auto&& arr : matrix) {
        log.Set_Values_WithId(arr);
        res << log;
    }
    return res;
}

bool Log::Clear_byType(int type) {
    if (type < 0) {
        return DB::SqlExe::Delete_allLines(table);
    }

    auto query = QString("DELETE FROM %1 where %2 = '%3' ");
    query      = query.arg(table.Get_tableName());
    query      = query.arg(table.Get_attributeList()[DB::TableEntity::log::type]);
    query      = query.arg(ENTITY::Log::Get_logTypeString(type));
    return DB::SqlExe::Get_ByOutSql(query).size();
}

}  // namespace CONTROLLER

#include "DB_SqlExe.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlRecord>
#include <QStringList>

#include "../impl/DB_DBEntity.h"

namespace {
template <typename T>
QString merge_strList(T &&msgList) {
    QStringList strList;
    for (auto &&s : msgList) {
        strList << ("'" + s + "'");
    }
    return strList.join(',');
}

std::mutex db_mutex;
}  // namespace

namespace DB {
QSqlQuery SqlExe::get_sqlQueryObj() {
    return QSqlQuery(DB::DBEntity::Get_connection());
}

QList<QList<QString> > SqlExe::Get_ByOutSql(const QString &query) {
    std::lock_guard lock(db_mutex);
    auto            sql = ::DB::SqlExe::get_sqlQueryObj();
    if (!sql.exec(query)) {
        qWarning() << sql.lastError() << query;
        return {};
    }

    QList<QList<QString> > sqlTable;
    while (sql.next()) {
        QList<QString> line;
        for (int i = 0; i < sql.record().count(); i += 1) {
            line.push_back(sql.value(i).toString());
        }
        sqlTable.push_back(std::move(line));
    }

    return sqlTable;
}

/****************
 * IEnumBase
 ****************/
QList<QList<QString> > SqlExe::Get_all_value(const DB::TableEntity::IEntityBase &table) {
    std::lock_guard lock(db_mutex);
    auto            sql   = ::DB::SqlExe::get_sqlQueryObj();
    auto            query = QString(" SELECT * FROM %1");
    query                 = query.arg(table.Get_tableName());

    if (!sql.exec(query)) {
        qWarning() << sql.lastError() << query;
        return {};
    }

    QList<QList<QString> > sqlTable;
    while (sql.next()) {
        QList<QString> line;
        for (int i = 0; i < sql.record().count(); i += 1) {
            line.push_back(sql.value(i).toString());
        }
        sqlTable.push_back(std::move(line));
    }

    return sqlTable;
}

QList<QList<QString> > SqlExe::Get_between(const TableEntity::IEntityBase &table, const int start,
                                           const int end) {
    std::lock_guard lock(db_mutex);
    auto            sql   = ::DB::SqlExe::get_sqlQueryObj();
    auto            query = QString("select * from %1 limit %2 offset %3;");
    query                 = query.arg(table.Get_tableName());

    const int limit  = end - start + 1;
    const int offset = start - 1;
    query            = query.arg(limit).arg(offset);

    if (!sql.exec(query)) {
        qWarning() << sql.lastError() << query;
        return {};
    }

    QList<QList<QString> > sqlTable;
    while (sql.next()) {
        QList<QString> line;
        for (int i = 0; i < sql.record().count(); i += 1) {
            line.push_back(sql.value(i).toString());
        }
        sqlTable.push_back(std::move(line));
    }

    return sqlTable;
}

int SqlExe::Get_all_count(const TableEntity::IEntityBase &table) {
    std::lock_guard lock(db_mutex);
    auto            sql   = ::DB::SqlExe::get_sqlQueryObj();
    auto            query = QString(" SELECT count(*) FROM %1");
    query                 = query.arg(table.Get_tableName());

    if (!sql.exec(query)) {
        qWarning() << sql.lastError() << query;
        return {};
    }

    if (sql.next()) {
        return sql.value(0).toInt();
    } else {
        return 0;
    }
}

QString SqlExe::Get_keyValue(const TableEntity::IEKeyVal &table, int key) {
    std::lock_guard lock(db_mutex);
    auto            sql = ::DB::SqlExe::get_sqlQueryObj();

    auto query = QString(" SELECT value FROM %1 WHERE key = '%2'; ");
    query      = query.arg(table.Get_tableName());

    if (!sql.exec(query.arg(table.Get_keyList()[key]))) {
        qWarning() << sql.lastError() << query;
        return "";
    }

    sql.next();
    return sql.value(0).toString();
}

bool SqlExe::Set_keyValue(const TableEntity::IEKeyVal &table, int key, const QString &value) {
    std::lock_guard lock(db_mutex);
    auto            sql = ::DB::SqlExe::get_sqlQueryObj();

    auto query = QString(" UPDATE %1 SET value = '%2' WHERE key = '%3'; ");
    query      = query.arg(table.Get_tableName());
    query      = query.arg(value);
    query      = query.arg(table.Get_keyList()[key]);

    if (!sql.exec(query)) {
        qWarning() << sql.lastError() << query;
        return false;
    }

    return true;
}

/****************
 * IAttributeEnum
 ****************/
bool SqlExe::Insert_oneLine(const TableEntity::IEAttribute &table,
                            const QVector<QString>         &lineMsg) {
    if (lineMsg.size() == 0) {
        return true;
    }

    std::lock_guard lock(db_mutex);
    auto            sql = get_sqlQueryObj();
    QString         query;

    /// 全部插入可以省略键值
    // QString(" INSERT INTO %1 VALUES ('%2', '%3', '%4', '%5', '%6', '%7');");
    query = QString(" INSERT INTO %1 VALUES ( %2 ");
    query = query.arg(table.Get_tableName());
    for (int i = 0; i < lineMsg.size(); i += 1) {
        QString placeholder = "'" + lineMsg[i];
        /// 最后一个没有 ,
        if (i + 1 == lineMsg.size()) {
            placeholder += "'  );";
        } else {
            placeholder += "', %1";
        }
        query = query.arg(placeholder);
    }

    return sql.exec(query);
}

bool SqlExe::Delete_oneLine_byId(const TableEntity::IEAttribute &table, const QString &name) {
    std::lock_guard lock(db_mutex);
    auto            sql   = ::DB::SqlExe::get_sqlQueryObj();
    QString         query = QString("delete from %1 where %2 = '%3';");
    query                 = query.arg(table.Get_tableName());
    query                 = query.arg(table.Get_attributeList()[0]);
    query                 = query.arg(name);

    return sql.exec(query);
}

QVector<QString> SqlExe::Select_oneLine_byName(const TableEntity::IEAttribute &table,
                                               const QString                  &name) {
    std::lock_guard lock(db_mutex);
    auto            sql   = ::DB::SqlExe::get_sqlQueryObj();
    QString         query = QString("select * from %1 where %2 = '%3';");
    query                 = query.arg(table.Get_tableName());
    query                 = query.arg(table.Get_attributeList()[0]);
    query                 = query.arg(name);

    if (false == sql.exec(query)) {
        qWarning() << sql.lastError() << query;
        return {};
    }

    QVector<QString> msgList;
    if (sql.next()) {
        for (int i = 0; i < sql.record().count(); i += 1) {
            msgList << sql.value(i).toString();
        }
    }
    return msgList;
}

bool SqlExe::Has_oneLine_byName(const TableEntity::IEAttribute &table, const QString &name) {
    return Select_oneLine_byName(table, name)[0].size() != 0;
}

bool SqlExe::Update_oneLine_byId(const TableEntity::IEAttribute &table,
                                 const QList<QString>           &lineMsg) {
    std::lock_guard lock(db_mutex);
    auto            sql     = ::DB::SqlExe::get_sqlQueryObj();
    auto          &&keyList = table.Get_attributeList();
    if (keyList.size() != lineMsg.size()) {
        qWarning() << "长度不对应" << lineMsg;
        return false;
    }

    QString query;
    query = QString("update '%1' set %4 where %2 = '%3';");
    query = query.arg(table.Get_tableName());
    query = query.arg(keyList[0]).arg(lineMsg[0]);

    for (int i = 1; i < lineMsg.size(); i += 1) {
        QString place = keyList[i] + " = '" + lineMsg[i] + "' ";
        if (i + 1 != lineMsg.size()) {
            place += ", %1 ";
        }
        query = query.arg(place);
    }

    return sql.exec(query);
}

/****************
 * IAutoincrementEnum
 ****************/
int SqlExe::Insert_oneLine(const TableEntity::IEAutoIncrement &table,
                           const QList<QString>               &lineMsg) {
    if (table.Get_attributeList().size() != lineMsg.size()) {
        qWarning() << "长度不对应" << lineMsg;
        return false;
    }

    std::lock_guard lock(db_mutex);
    auto            sql = get_sqlQueryObj();
    QString         query;

    // INSERT INTO example (name, value) VALUES ('Example Name', 123);
    QString key = merge_strList(table.Get_attributeList());
    QString val = merge_strList(lineMsg);
    query       = QString("insert into %1 (%2) values (%3);")  //
                .arg(table.Get_tableName())                    //
                .arg(key)                                      //
                .arg(val)                                      //
        ;                                                      //

    if (false == sql.exec(query)) {
        return 0;
    }

    /**
     * 作用：在主键自增的时候获取最近操作的主键
     * 性质：这个不是执行普通的sql，而是可以直接从sqlite内部读取
     * 注意：这个操作与具体表无关，因为落实到sqlite的实体了，
     *       就是说table1进行了insert，table2进行了insert
     *       此时读取的就是操作了后续的table2的状态
     *       因此应用程序的层面不是线程安全的
     */
    return sql.lastInsertId().toInt();
}

QList<QString> SqlExe::Select_oneLine_byId(const TableEntity::IEAutoIncrement &table, int id) {
    std::lock_guard lock(db_mutex);
    auto            sql = ::DB::SqlExe::get_sqlQueryObj();

    QString query = QString("select * from %1 where id = '%2';")  //
                        .arg(table.Get_tableName())               //
                        .arg(id)                                  //
        ;                                                         //

    if (false == sql.exec(query)) {
        qWarning() << sql.lastError() << query;
        return {};
    }

    QList<QString> msgList;
    /// 第一行是属性
    if (sql.next()) {
        for (int i = 0; i < sql.record().count(); i += 1) {
            msgList << sql.value(i).toString();
        }
    }
    return msgList;
}

bool SqlExe::Update_oneLine_byId(const TableEntity::IEAutoIncrement &table,
                                 const QList<QString> &lineMsg, int id) {
    if (table.Get_attributeList().size() != lineMsg.size()) {
        qWarning() << "长度不对应" << lineMsg;
        return false;
    }

    std::lock_guard lock(db_mutex);
    auto            sql = get_sqlQueryObj();
    QString         query;

    // update users set 'username' = '324343', 'email' = 'www' where id = 1;
    QVector<QString> keys = table.Get_attributeList();
    query                 = QString("update %1 set %2").arg(table.Get_tableName());
    for (int i = 0; i < keys.size(); i += 1) {
        QString txt = QString("'%1' = '%2' ").arg(keys[i]).arg(lineMsg[i]);
        if (i + 1 < keys.size()) {
            txt += ",";
        }
        txt   += "%1";
        query  = query.arg(txt);
    }
    QString txt = QString(" where id = %1 ;").arg(id);
    query       = query.arg(txt);

    return sql.exec(query);
}

bool SqlExe::Delete_oneLine_byId(const TableEntity::IEAutoIncrement &table, int id) {
    std::lock_guard lock(db_mutex);
    auto            sql   = ::DB::SqlExe::get_sqlQueryObj();
    QString         query = QString("delete from %1 where id = '%2';");
    query                 = query.arg(table.Get_tableName());
    query                 = query.arg(id);

    return sql.exec(query);
}

bool SqlExe::Delete_allLines(const TableEntity::IEntityBase &table) {
    std::lock_guard lock(db_mutex);
    auto            sql   = ::DB::SqlExe::get_sqlQueryObj();
    QString         query = QString("DELETE FROM %1;").arg(table.Get_tableName());

    if (!sql.exec(query)) {
        qWarning() << sql.lastError() << query;
        return false;
    }
    return true;
}

}  // namespace DB

#include "DB_TableFactory_KeyValue.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>

namespace DB {

KeyValue::KeyValue(const QString &tableName, const QVector<QString> &keyList,
                   const QVector<QString> &valueList)
    : TableFactory(tableName) {
    this->m_keyList   = keyList;
    this->m_valueList = valueList;

    /// 必须保证两个的数量一致
    assert(keyList.size() == valueList.size());
}

/**
 * @brief KeyValue::Check_table
 * @return
 * 1. 是否存在表
 * 2. 表内的key的量是否对应
 */
bool KeyValue::Check_table() {
    if (isExist_table() == false) {
        if (create_table() == false) {
            return false;
        }
    }

    // 数据库里 所有的记录数
    if (false == check_tableStruct()) {
        if (false == Set_initTable()) {
            qWarning() << "Check_table Set_initTable 数据库 操作失败";
        }
    }

    return true;
}

/**
 * @brief KeyValue::create_table
 * @return
 * 建表，key value的sql都一样
 */
bool KeyValue::create_table() {
    QSqlQuery sql(m_dbConnect);

    auto query = QString(R"(
    CREATE TABLE IF NOT EXISTS %1 (
          key TEXT,
          value TEXT
    );
    )");
    query      = query.arg(m_tableName);

    qInfo() << query;
    if (!sql.exec(query)) {
        qWarning() << "create_table sql执行失败" << sql.lastError();
        return false;
    }
    return true;
}

/**
 * @brief KeyValue::check_tableStruct
 * @return
 * 检查键值的数量
 */
bool KeyValue::check_tableStruct() {
    QSqlQuery sql(m_dbConnect);

    auto query = QString(" SELECT COUNT(*) FROM %1; ");
    query      = query.arg(m_tableName);

    qInfo() << query;
    if (!sql.exec(query)) {
        qWarning() << sql.lastError();
        return 0;
    }
    /// 第一行是查询的表头
    sql.next();

    return m_keyList.size() == sql.value(0).toInt();
}

/**
 * @brief KeyValue::Set_initTable
 * @return
 * 设置key value表的默认值
 */
bool KeyValue::Set_initTable() {
    // 若不存在该表, 则创建
    if (!isExist_table()) {
        if (!create_table()) {
            return false;
        }
    }

    // 清空表所有数据
    if (!delete_table()) {
        return false;
    }

    bool isSucc = true;
    // 插入初始化数据
    for (int i = 0; i < m_keyList.size(); ++i) {
        if (false == insert_one(m_keyList.at(i), m_valueList.at(i))) {
            isSucc = false;
            break;
        }
    }
    return isSucc;
}

/**
 * @brief KeyValue::delete_table
 * @return
 * 这是清除表中的所有数据，而不是把表删除
 */
bool KeyValue::delete_table() {
    QSqlQuery sql(m_dbConnect);

    auto query = QString(" DELETE FROM %1; ");
    query      = query.arg(m_tableName);

    qInfo() << query;
    if (!sql.exec(query)) {
        qWarning() << sql.lastError();
        return false;
    }
    return true;
}

/**
 * @brief KeyValue::insert_one
 * @param key
 * @param value
 * @return
 * 插入单个key value
 */
bool KeyValue::insert_one(const QString &key, const QString &value) {
    QSqlQuery sql(m_dbConnect);

    auto query = QString(" INSERT INTO %1 (key, value) VALUES ('%2', '%3')");
    query      = query.arg(m_tableName);

    qInfo() << query;
    if (!sql.exec(query.arg(key).arg(value))) {
        qWarning() << sql.lastError();
        return false;
    }
    return true;
}
}  // namespace DB

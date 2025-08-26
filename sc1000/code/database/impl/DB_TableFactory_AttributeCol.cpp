#include "DB_TableFactory_AttributeCol.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace DB {

AttributeCol::AttributeCol(const QString &tableName, const QVector<QString> &attributeList)
    : TableFactory(tableName) {
    this->m_attributeList = attributeList;

    /// 不能为空
    assert(m_attributeList.size());
}

/**
 * @brief AttributeCol::Check_table
 * @return
 * 属性表的核心是列的数量
 * 列数不对就是结构不对
 * 要把整张表删除再重新创建
 */
bool AttributeCol::Check_table() {
    if (isExist_table() == false || check_tableStruct() == false) {
        if (drop_table() == false || create_table() == false) {
            return false;
        }
    }
    return true;
}

/**
 * @brief AttributeCol::check_tableStruct
 * @return
 * 检测列数
 */
bool AttributeCol::check_tableStruct() {
    QSqlQuery sql(m_dbConnect);
    QString   query = QString("select * from %1");
    query           = query.arg(m_tableName);

    if (!sql.exec(query)) {
        qWarning() << sql.lastError();
        return false;
    }

    return m_attributeList.size() == sql.record().count();
}

/**
 * @brief AttributeCol::drop_table
 * @return
 * 删除表是 drop 不是 delete
 */
bool AttributeCol::drop_table() {
    QSqlQuery sql(m_dbConnect);
    auto      query = QString(" DROP TABLE IF EXISTS %1; ");
    query           = query.arg(m_tableName);

    qInfo() << query;
    if (!sql.exec(query)) {
        qWarning() << sql.lastError();
        return false;
    }
    return true;
}

/**
 * @brief AttributeCol::create_table
 * @return
 * 建立表，根据属性的量决定
 */
bool AttributeCol::create_table() {
    QSqlQuery sql(m_dbConnect);

    //! 注意一下这里最后的 ")" 是原始R字符串的首尾，不是字符串本身的内容
    auto query = QString(R"( CREATE TABLE IF NOT EXISTS %1 ( %2 )");
    query      = query.arg(m_tableName);

    for (int i = 0; i < m_attributeList.size(); i += 1) {
        QString text = m_attributeList[i];
        if (i + 1 < m_attributeList.size()) {
            text += " TEXT, %1 ";
        } else {
            text += " TEXT );";
        }
        query = query.arg(text);
    }

    qInfo() << query;
    if (!sql.exec(query)) {
        qWarning() << sql.lastError();
        return false;
    }
    return true;
}

}  // namespace DB

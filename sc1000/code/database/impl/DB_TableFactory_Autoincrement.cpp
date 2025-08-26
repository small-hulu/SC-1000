#include "DB_TableFactory_Autoincrement.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace DB {

bool Autoincrement::create_table() {
    QSqlQuery sql(m_dbConnect);

    //! 注意一下这里最后的 ")" 是原始R字符串的首尾，不是字符串本身的内容
    auto query = QString(R"( CREATE TABLE %1 ( id INTEGER PRIMARY KEY AUTOINCREMENT, %2 )");
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

bool Autoincrement::check_tableStruct() {
    QSqlQuery sql(m_dbConnect);
    QString   query = QString("select * from %1");
    query           = query.arg(m_tableName);

    // qInfo() << query;
    if (!sql.exec(query)) {
        qWarning() << sql.lastError();
        return false;
    }

    /// id + 属性数量
    return m_attributeList.size() + 1 == sql.record().count();
}

}  // namespace DB

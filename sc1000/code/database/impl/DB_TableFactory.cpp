#include "DB_TableFactory.h"

#include <QDebug>
#include <QSqlError>

#include "DB_DBEntity.h"

namespace DB {

TableFactory ::TableFactory(const QString &tableName) {
    this->m_dbConnect = DBEntity::Get_connection();
    this->m_tableName = tableName;
}

bool TableFactory ::isExist_table() {
    if (m_dbConnect.isOpen() == false) {
        qCritical() << "isExist_table db执行失败" << m_dbConnect.lastError().text();
        return false;
    }
    return m_dbConnect.tables().contains(m_tableName);
}

}  // namespace DB

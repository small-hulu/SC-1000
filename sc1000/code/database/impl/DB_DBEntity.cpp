#include "DB_DBEntity.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSqlError>

namespace DB {

QSqlDatabase DBEntity::global_systemDB_Entity;

/**
 * @brief DBController::Open
 * @param DBFilePath    .db文件路径
 * @param connectName   连接名
 * @return
 * 只准调用一次
 */
bool DBEntity::Open(const QString &DBFilePath, const QString &connectName) {
    static bool once = true;
    assert(once);
    once = false;

    // 如果根据这个路径, 先去创建(文件夹)  [WriteOnly, 不能创建文件夹]
    QFile file(DBFilePath);
    if (!file.exists() && !file.open(QIODevice::WriteOnly)) {
        qWarning() << ("System系统数据库 文件, 创建失败 \n, 错误原因: " + file.errorString());
        return false;
    }

    QSqlDatabase db;
    if (QSqlDatabase::contains(connectName)) {
        db = QSqlDatabase::database(connectName, true);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectName);
        db.setDatabaseName(DBFilePath);
    }

    if (db.open() == false) {
        qCritical() << "System系统数据库, 打开失败, " << db.lastError().text();
        return false;
    }

    // attention
    // 这里, 数据库对象 也可以copy!!!
    // 即a=_db, _db是open了的, 则a也是open的
    global_systemDB_Entity = db;
    return true;
}

bool DBEntity::Close() {
    auto &&db = global_systemDB_Entity;
    if (db.isOpen()) {
        db.close();
    }
    return db.isOpen();
}

/**
 * @brief DBEntity::Get_connection
 * @return
 * 所有sql操作都要依赖于此
 */
const QSqlDatabase &DBEntity::Get_connection() {
    return global_systemDB_Entity;
}

}  // namespace DB

#include "DB_Init.h"

#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include "extend/DB_TableCheck.h"
#include "impl/DB_DBEntity.h"

namespace DB {

namespace {
const char* const connectName = "systemDb";
}  // namespace

bool Init(const QString& dbfolder, const QString& dbFile) {
    //! 文件夹和文件名合并
    const QString dbFilePath = QDir(dbfolder).filePath(dbFile);

    /// 打开数据库连接
    /// 数据库的位置固定，不要让用户改动
    if (false == DB::DBEntity::Open(dbFilePath, connectName)) {
        qCritical() << "数据库Open错误";
        QMessageBox::critical(nullptr, "Database Error", dbFilePath + " database link error");
        return false;
    }

    /// 检查该数据库文件里的 所有表
    if (false == TableCheck::Check_allTable()) {
        qCritical() << "数据库Check错误";
        QMessageBox::critical(nullptr, "Database Error", "Database Check Fail");
        return false;
    }

    return true;
}

bool Exit() {
    return DB::DBEntity::Close();
}

}  // namespace DB

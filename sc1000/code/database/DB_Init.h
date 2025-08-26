#ifndef DB_INIT_H_1688377776_1698298597
#define DB_INIT_H_1688377776_1698298597

#include <QString>

namespace DB {

/**
 * 请保证传入的`folder`已经创建
 */
extern bool Init(const QString& dbfolder, const QString& dbFile);

extern bool Exit();

}  // namespace DB

#endif  // DB_INIT_H_1688377776_1698298597

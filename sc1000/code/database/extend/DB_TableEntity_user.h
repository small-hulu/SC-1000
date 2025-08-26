#ifndef USER_H_1719886447
#define USER_H_1719886447

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {
struct user : IEAutoIncrement {
    enum table_attribute : int {
        username = 0,  // 账户
        password,      // 密码
        level,         // 权限等级
        createtime,    // 创建时间
        remark,        // 备注

        Enum_Count
    };

    QString                 Get_tableName() const override;
    QString                 Get_chineseEnum(int idx) const override;
    const QVector<QString>& Get_attributeList() const override;
};
}  // namespace TableEntity
}  // namespace DB

#endif  // USER_H_1719886447

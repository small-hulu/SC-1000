#include "DB_TableEntity_user.h"

namespace DB {
namespace TableEntity {
QString user::Get_tableName() const {
    return "user";
}

QString user::Get_chineseEnum(int idx) const {
    table_attribute e{idx};
    switch (e) {
    case username  : return "账户";
    case password  : return "密码";
    case level     : return "权限等级";
    case createtime: return "创建时间";
    case remark    : return "备注";
    case Enum_Count: return "";

    default        : return "";
    }
}

const QVector<QString>& user::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine = __LINE__;
    enumList[username]      = "username";
    enumList[password]      = "password";
    enumList[level]         = "level";
    enumList[createtime]    = "createtime";
    enumList[remark]        = "remark";
    constexpr int endLine   = __LINE__;

    static_assert(Enum_Count == (endLine - startLine - 1));
    return enumList;
}

}  // namespace TableEntity
}  // namespace DB

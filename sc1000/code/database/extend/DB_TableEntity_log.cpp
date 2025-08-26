#include "DB_TableEntity_log.h"

namespace DB {
namespace TableEntity {

QString log::Get_tableName() const {
    return "log";
}

QString log::Get_chineseEnum(int idx) const {
    table_attribute e{idx};
    switch (e) {
    case message   : return "信息";
    case type      : return "日志类型";
    case time      : return "时间";
    case file      : return "文件";
    case func      : return "函数名";
    case line      : return "行号";
    case remark    : return "备注";
    case Enum_Count: return "";

    default        : return "";
    }
}

const QVector<QString> &log::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine = __LINE__;
    enumList[message]       = "message";
    enumList[type]          = "type";
    enumList[time]          = "time";
    enumList[file]          = "file";
    enumList[func]          = "func";
    enumList[line]          = "line";
    enumList[remark]        = "remark";
    constexpr int endLine   = __LINE__;

    static_assert(Enum_Count == (endLine - startLine - 1));
    return enumList;
}

}  // namespace TableEntity
}  // namespace DB

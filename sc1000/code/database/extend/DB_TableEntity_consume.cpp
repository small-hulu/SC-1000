#include "DB_TableEntity_consume.h"

namespace DB::TableEntity {

QString consume::Get_tableName() const {
    return "consume";
}

QString consume::Get_chineseEnum(int idx) const {
    table_attribute e{idx};
    switch (e) {
    case type      : return "类型";
    case qrcode    : return "条码";
    case usedVal   : return "使用量";
    case time      : return "时间";
    case remark    : return "备注";
    case Enum_Count: return "";

    default        : return "";
    }
}

const QVector<QString> &consume::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine = __LINE__;
    enumList[type]          = "type";
    enumList[qrcode]        = "qrcode";
    enumList[usedVal]       = "usedVal";
    enumList[time]          = "time";
    enumList[remark]        = "remark";
    constexpr int endLine   = __LINE__;

    static_assert(Enum_Count == (endLine - startLine - 1));
    return enumList;
}

}  // namespace DB::TableEntity

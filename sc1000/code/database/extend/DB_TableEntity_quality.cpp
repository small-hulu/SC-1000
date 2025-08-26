#include "DB_TableEntity_quality.h"

namespace DB {
namespace TableEntity {

QString quality::Get_tableName() const {
    return "quality";
}

QString quality::Get_chineseEnum(int idx) const {
    auto e = static_cast<table_attribute>(idx);
    switch (e) {
    case state           : return "状态";
    case qrcode          : return "条码";
    case project         : return "项目";
    case name            : return "质控名称";
    case batchNumber     : return "批号";
    case bottleNumber    : return "瓶号";
    case idReagent       : return "试剂id";
    case location        : return "位置";
    case timeStart       : return "申请时间";
    case timeEnd         : return "结束时间";
    case productDate     : return "生产日期";
    case periodOfValidity: return "有效期";
    case valResult       : return "结果";
    case valTarget       : return "靶值";
    case valTargetStart  : return "靶值起点";
    case valTargetEnd    : return "靶值终点";
    case valSD           : return "SD值";
    case unit            : return "单位";
    case mark            : return "标记";
    case rule            : return "规则";
    case Enum_Count      :;
    }
    return "Error-OutOfRange";
}

const QVector<QString> &quality::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine    = __LINE__;
    enumList[state]            = "state";
    enumList[qrcode]           = "qrcode";
    enumList[project]          = "project";
    enumList[name]             = "name";
    enumList[batchNumber]      = "batchNumber";
    enumList[bottleNumber]     = "bottleNumber";
    enumList[idReagent]        = "idReagent";
    enumList[location]         = "location";
    enumList[timeStart]        = "timeStart";
    enumList[timeEnd]          = "timeEnd";
    enumList[productDate]      = "productDate";
    enumList[periodOfValidity] = "periodOfValidity";
    enumList[valResult]        = "valResult";
    enumList[valTarget]        = "valTarget";
    enumList[valTargetStart]   = "valTargetStart";
    enumList[valTargetEnd]     = "valTargetEnd";
    enumList[valSD]            = "valSD";
    enumList[unit]             = "unit";
    enumList[mark]             = "mark";
    enumList[rule]             = "rule";
    constexpr int endLine      = __LINE__;

    static_assert(Enum_Count == (endLine - startLine - 1));
    return enumList;
}

}  // namespace TableEntity
}  // namespace DB

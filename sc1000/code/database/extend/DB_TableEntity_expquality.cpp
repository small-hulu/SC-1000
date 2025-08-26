#include "DB_TableEntity_expquality.h"

namespace DB {
namespace TableEntity {

QString expquality::Get_tableName() const {
    return "expquality";
}

QString expquality::Get_chineseEnum(int idx) const {
    auto e = static_cast<table_attribute>(idx);
    switch (e) {
    case state     : return "状态标记";
    case isLoad    : return "是否装载";
    case idQuality : return "质控品id";
    case idReagent : return "试剂id";
    case name      : return "质控名称";
    case batchNum  : return "批号";
    case timeStart : return "申请时间";
    case timeEnd   : return "结束时间";
    case location  : return "位置";
    case lis       : return "LIS";
    case project   : return "项目";
    case valResult : return "结果";
    case unit      : return "单位";
    case lightVal  : return "发光值";
    case mark      : return "标记";
    case rule      : return "规则";
    case valTarget : return "靶值";
    case valSD     : return "SD值";

    case Enum_Count:;
    }
    return "Error-OutOfRange";
}

const QVector<QString> &expquality::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine = __LINE__;
    enumList[state]         = "state";
    enumList[isLoad]        = "isLoad";
    enumList[idQuality]     = "idQuality";
    enumList[idReagent]     = "idReagent";
    enumList[name]          = "name";
    enumList[batchNum]      = "batchNum";
    enumList[timeStart]     = "timeStart";
    enumList[timeEnd]       = "timeEnd";
    enumList[location]      = "location";
    enumList[lis]           = "lis";
    enumList[project]       = "project";
    enumList[valResult]     = "valResult";
    enumList[unit]          = "unit";
    enumList[lightVal]      = "lightVal";
    enumList[mark]          = "mark";
    enumList[rule]          = "rule";
    enumList[valTarget]     = "valTarget";
    enumList[valSD]         = "valSD";
    constexpr int endLine   = __LINE__;

    static_assert(Enum_Count == (endLine - startLine - 1));
    return enumList;
}

}  // namespace TableEntity
}  // namespace DB

#include "DB_TableEntity_expsample.h"

namespace DB {
namespace TableEntity {

QString expsample::Get_tableName() const {
    return "expsample";
}

QString expsample::Get_chineseEnum(int idx) const {
    table_attribute e{idx};
    switch (e) {
    case state     : return "状态标记";
    case idSample  : return "样本id";
    case idReagent : return "试剂id";
    case type      : return "类型";
    case project   : return "项目";
    case expData   : return "发光值";
    case resultData: return "浓度值";
    case timeStart : return "开始时间";
    case timeEnd   : return "结束时间";
    case remark    : return "备注";
    case Enum_Count:;
    default        :;
    }
    return "";
}

const QVector<QString> &expsample::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine = __LINE__;
    enumList[state]         = "state";
    enumList[idSample]      = "idSample";
    enumList[idReagent]     = "idReagent";
    enumList[type]          = "type";
    enumList[project]       = "project";
    enumList[expData]       = "expData";
    enumList[resultData]    = "resultData";
    enumList[timeStart]     = "timeStart";
    enumList[timeEnd]       = "timeEnd";
    enumList[remark]        = "remark";
    constexpr int endLine   = __LINE__;

    return enumList;
    static_assert(Enum_Count == (endLine - startLine - 1));
}

}  // namespace TableEntity
}  // namespace DB

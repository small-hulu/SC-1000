#include "DB_TableEntity_reagent.h"

namespace DB {
namespace TableEntity {
QString reagent::Get_tableName() const {
    return "reagent";
}

QString reagent::Get_chineseEnum(int idx) const {
    table_attribute e{idx};
    switch (e) {
    case state                : return "状态标记";
    case isLoad               : return "是否装载";
    case qrcode               : return "条码";
    case idExpCal             : return "校准对应编号";
    case location             : return "位置";
    case project              : return "项目";
    case type                 : return "类型";
    case filling              : return "试剂灌注量";
    case liquidConsume        : return "试剂消耗量";
    case ableMeasureBottleCnt : return "瓶可测数";
    case ableProjectMeasureCnt: return "项目可测数";
    case periodOfValidity     : return "有效期";
    case batchNum             : return "批号";
    case bottleNum            : return "瓶号";
    case bottleOpenTime       : return "开瓶时间";
    case calibrationStatus    : return "校准状态";
    case mainCurve            : return "主曲线";
    case A                    : return "A";
    case B                    : return "B";
    case C                    : return "C";
    case D                    : return "D";
    case measuredTimes        : return "已测次数";

    case remark               : return "备注";
    case Enum_Count           : return "";

    default                   : return "";
    }
}

const QVector<QString>& reagent::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine         = __LINE__;
    enumList[state]                 = "state";
    enumList[isLoad]                = "isLoad";
    enumList[qrcode]                = "qrcode";
    enumList[idExpCal]              = "idExpCal";
    enumList[location]              = "location";
    enumList[project]               = "project";
    enumList[type]                  = "type";
    enumList[filling]               = "filling";
    enumList[liquidConsume]         = "liquidConsume";
    enumList[ableMeasureBottleCnt]  = "ableMeasureBottleCnt";
    enumList[ableProjectMeasureCnt] = "ableProjectMeasureCnt";
    enumList[periodOfValidity]      = "periodOfValidity";
    enumList[batchNum]              = "batchNum";
    enumList[bottleNum]             = "bottleNum";
    enumList[bottleOpenTime]        = "bottleOpenTime";
    enumList[calibrationStatus]     = "calibrationStatus";
    enumList[mainCurve]             = "mainCurve";
    enumList[A]                     = "A";
    enumList[B]                     = "B";
    enumList[C]                     = "C";
    enumList[D]                     = "D";
    enumList[measuredTimes]         = "measuredTimes";
    enumList[remark]                = "remark";
    constexpr int endLine           = __LINE__;

    static_assert(Enum_Count == (endLine - startLine - 1));
    return enumList;
}
}  // namespace TableEntity
}  // namespace DB

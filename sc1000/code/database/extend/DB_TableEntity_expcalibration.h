#ifndef EXPCALIBRATION_H_1720577517
#define EXPCALIBRATION_H_1720577517

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {
struct expcalibration : IEAutoIncrement {
    enum table_attribute : int {
        state,            // 状态标记
        isLoad,           // 是否装载
        idReagent,        // 试剂id
        idCalibration,    // 校准品id
        project,          // 项目
        timeStart,        // 申请时间
        timeEnd,          // 结束时间
        C0nLocationList,  // c0~cn的位置
        C0nLightValList,  // c0~cn的发光值

        remark,  // 备注

        Enum_Count
    };

    QString Get_tableName() const override {
        return "expcalibration";
    }
    QString Get_chineseEnum(int idx) const override {
        table_attribute e{idx};
        switch (e) {
        case state          : return "状态标记";
        case isLoad         : return "是否装载";
        case idReagent      : return "试剂id";
        case idCalibration  : return "校准品id";
        case project        : return "项目";
        case timeStart      : return "申请时间";
        case timeEnd        : return "结束时间";
        case C0nLocationList: return "C0nLocationList";
        case C0nLightValList: return "C0nLightValList";
        case remark         : return "remark";
        case Enum_Count     :;
        }

        return "Enum-OutOfRange";
    }
    const QVector<QString>& Get_attributeList() const override {
        static QVector<QString> enumList;
        if (enumList.size()) {
            return enumList;
        }
        enumList = QVector<QString>(Enum_Count);

        constexpr int startLine   = __LINE__;
        enumList[state]           = "state";
        enumList[isLoad]          = "isLoad";
        enumList[idReagent]       = "idReagent";
        enumList[idCalibration]   = "idCalibration";
        enumList[project]         = "project";
        enumList[timeStart]       = "timeStart";
        enumList[timeEnd]         = "timeEnd";
        enumList[C0nLocationList] = "C0nLocationList";
        enumList[C0nLightValList] = "C0nLightValList";
        enumList[remark]          = "remark";
        constexpr int endLine     = __LINE__;

        static_assert(Enum_Count == (endLine - startLine - 1));
        return enumList;
    }
};
}  // namespace TableEntity
}  // namespace DB
#endif  // EXPCALIBRATION_H_1720577517

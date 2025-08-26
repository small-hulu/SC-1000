#ifndef CALIBRATION_H_1720064848
#define CALIBRATION_H_1720064848

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {
struct calibration : IEAutoIncrement {
    enum table_attribute : int {
        qrcode,                // 条码
        state,                 // 状态标记
        name,                  // 名字
        project,               // 项目
        batchNum,              // 校准品批号
        substrateBtanch,       // 底物批号
        substrateBottle,       // 底物瓶号
        timeStart,             // 申请时间
        timeEnd,               // 结束时间
        dateProduction,        // 生产日期
        dateExpiration,        // 保质期
        typeTube,              // 样本管型号
        stateCalibration,      // 校准品状态
        markCalibration,       // 校准标记
        targetConcentration,   // 目标浓度
        mark,                  // 标记
        C0nConcentrationList,  // c0n浓度

        Enum_Count
    };

    QString Get_tableName() const override;

    QString Get_chineseEnum(int idx) const override {
        table_attribute e{idx};
        switch (e) {
        case qrcode              : return "条码";
        case state               : return "状态标记";
        case name                : return "名字";
        case project             : return "项目";
        case batchNum            : return "校准品批号";
        case substrateBtanch     : return "底物批号";
        case substrateBottle     : return "底物瓶号";
        case timeStart           : return "申请时间";
        case timeEnd             : return "结束时间";
        case dateProduction      : return "生产日期";
        case dateExpiration      : return "保质期";
        case typeTube            : return "样本管型号";
        case stateCalibration    : return "校准品状态";
        case markCalibration     : return "校准标记";
        case targetConcentration : return "目标浓度";
        case mark                : return "标记";
        case C0nConcentrationList: return "c0n浓度";
        case Enum_Count          :;
        }

        return "Enum-OutOfRange";
    }

    const QVector<QString>& Get_attributeList() const override {
        static QVector<QString> enumList;
        if (enumList.size()) {
            return enumList;
        }
        enumList = QVector<QString>(Enum_Count);

        constexpr int startLine        = __LINE__;
        enumList[qrcode]               = "qrcode";
        enumList[state]                = "state";
        enumList[name]                 = "name";
        enumList[project]              = "project";
        enumList[batchNum]             = "batchNum";
        enumList[substrateBtanch]      = "substrateBtanch";
        enumList[substrateBottle]      = "substrateBottle";
        enumList[timeStart]            = "timeStart";
        enumList[timeEnd]              = "timeEnd";
        enumList[dateProduction]       = "dateProduction";
        enumList[dateExpiration]       = "dateExpiration";
        enumList[typeTube]             = "typeTube";
        enumList[stateCalibration]     = "stateCalibration";
        enumList[markCalibration]      = "markCalibration";
        enumList[targetConcentration]  = "targetConcentration";
        enumList[mark]                 = "mark";
        enumList[C0nConcentrationList] = "C0nConcentration";
        constexpr int endLine          = __LINE__;

        static_assert(Enum_Count == (endLine - startLine - 1));
        return enumList;
    }
};
}  // namespace TableEntity
}  // namespace DB

#endif  // CALIBRATION_H_1720064848

#ifndef REAGENT_H_1719886585
#define REAGENT_H_1719886585

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {

struct reagent : IEAutoIncrement {
    enum table_attribute : int {
        state,                  // 状态标记
        isLoad,                 // 是否装载中
        qrcode,                 // 条码
        idExpCal,               // 校准对应编号
        location,               // 位置
        project,                // 项目
        type,                   // 类型
        filling,                // 0-3腔体理论灌注量,单位ml
        liquidConsume,          // 0-3腔体液体消耗量,单位ul
        ableMeasureBottleCnt,   // 瓶可测数
        ableProjectMeasureCnt,  // 项目可测数
        periodOfValidity,       // 有效期
        batchNum,               // 批号
        bottleNum,              // 瓶号
        bottleOpenTime,         // 开瓶时间
        calibrationStatus,      // 校准状态
        mainCurve,              // 主曲线
        A,                      // 主曲线的四参数A
        B,                      // 主曲线的四参数B
        C,                      // 主曲线的四参数C
        D,                      // 主曲线的四参数D
        measuredTimes,          // 已测次数

        remark,  // 备注

        Enum_Count
    };

    QString                 Get_tableName() const override;
    QString                 Get_chineseEnum(int idx) const override;
    const QVector<QString>& Get_attributeList() const override;
};

}  // namespace TableEntity
}  // namespace DB
#endif  // REAGENT_H_1719886585

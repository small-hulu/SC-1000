#ifndef QUALITY_H_1720064620
#define QUALITY_H_1720064620

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {

struct quality : IEAutoIncrement {
    enum table_attribute : int {
        state,             // 状态
        qrcode,            // 条码
        project,           // 项目
        name,              // 质控名称
        batchNumber,       // 批号
        bottleNumber,      // 瓶号
        idReagent,         // 试剂id
        location,          // 位置
        timeStart,         // 申请时间
        timeEnd,           // 结束时间
        productDate,       // 生产日期
        periodOfValidity,  // 有效期
        valResult,         // 结果
        valTarget,         // 靶值
        valTargetStart,    // 靶值起点
        valTargetEnd,      // 靶值终点
        valSD,             // SD值
        unit,              // 单位
        mark,              // 标记
        rule,              // 规则

        Enum_Count
    };

    QString                 Get_tableName() const override;
    QString                 Get_chineseEnum(int idx) const override;
    const QVector<QString>& Get_attributeList() const override;
};

}  // namespace TableEntity
}  // namespace DB

#endif  // QUALITY_H_1720064620

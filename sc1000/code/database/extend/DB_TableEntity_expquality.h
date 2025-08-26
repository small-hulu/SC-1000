#ifndef EXPQUALITY_H_1721286729
#define EXPQUALITY_H_1721286729
#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {

struct expquality : IEAutoIncrement {
    enum table_attribute : int {
        state,      // 状态标记
        isLoad,     // 是否装载
        idQuality,  // 质控品id
        idReagent,  // 试剂id
        name,       // 质控名称
        batchNum,   // 批号
        timeStart,  // 申请时间
        timeEnd,    // 结束时间
        location,   // 位置
        lis,        // LIS
        project,    // 项目
        valResult,  // 结果
        unit,       // 单位
        lightVal,   // 发光值
        mark,       // 标记
        rule,       // 规则
        valTarget,  // 靶值
        valSD,      // SD值

        Enum_Count
    };
    QString                 Get_tableName() const override;
    QString                 Get_chineseEnum(int idx) const override;
    const QVector<QString>& Get_attributeList() const override;
};

}  // namespace TableEntity
}  // namespace DB
#endif  // EXPQUALITY_H

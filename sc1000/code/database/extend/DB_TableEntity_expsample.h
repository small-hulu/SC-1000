#ifndef EXPERIMENT_H_1719892048
#define EXPERIMENT_H_1719892048

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {

struct expsample : IEAutoIncrement {
    enum table_attribute : int {
        state,       // 状态标记
        idSample,    // 样本id
        idReagent,   // 试剂的id
        type,        // 类型
        project,     // 项目
        expData,     // 发光值
        resultData,  // 浓度值
        timeStart,   // 开始时间
        timeEnd,     // 结束时间

        remark,  // 备注
        Enum_Count
    };

    QString Get_tableName() const override;

    QString Get_chineseEnum(int idx) const override;

    const QVector<QString>& Get_attributeList() const override;
};
}  // namespace TableEntity
}  // namespace DB
#endif  // EXPERIMENT_H_1719892048

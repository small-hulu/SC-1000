#ifndef KEYENUM_H_1688445072_1719886720_1731397122
#define KEYENUM_H_1688445072_1719886720_1731397122

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {

struct sample_apply : IEAutoIncrement {
    enum table_attribute : int {
        state,        // 状态标记
        idSample,     // 样本id
        projectList,  // 项目列表
        remark,       // 备注
        Enum_Count
    };

    QString                 Get_tableName() const override;
    QString                 Get_chineseEnum(int idx) const override;
    const QVector<QString>& Get_attributeList() const override;
};

}  // namespace TableEntity
}  // namespace DB
#endif  // KEYENUM_H_1688445072_1719886720_1731397122

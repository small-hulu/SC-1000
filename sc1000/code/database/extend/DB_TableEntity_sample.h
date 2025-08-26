#ifndef KEYENUM_H_1688445072_1719886720
#define KEYENUM_H_1688445072_1719886720

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {

struct sample : IEAutoIncrement {
    enum table_attribute : int {
        state,           // 状态标记
        isLoad,          // 是否装载
        qrcode,          // 条码
        project,         // 项目
        location,        // 位置
        number,          // 编号
        repeatCnt,       // 重复次数
        detectType,      // 检测类型
        sampleTube,      // 样本管（管子的类型）
        dilutionRatio,   // 稀释倍数
        emergency_sign,  // 急诊位
        expIdList,       // 实验id列表
        expValueList,    // 实验状态列表

        remark,  // 备注

        patient_name,        // 患者姓名
        patient_ageNum,      // 患者年龄
        patient_ageUnit,     // 患者年龄的单位
        patient_sex,         // 患者性别
        patient_patientId,   // 患者病人id
        patient_department,  // 患者科室
        patient_bedId,       // 患者床号
        patient_inspection,  // 患者送检者
        patient_verifier,    // 患者检验者
        patient_auditor,     // 患者审核者
        patient_remark,      // 患者备注

        Enum_Count
    };

    QString                 Get_tableName() const override;
    QString                 Get_chineseEnum(int idx) const override;
    const QVector<QString>& Get_attributeList() const override;
};

}  // namespace TableEntity
}  // namespace DB
#endif  // KEYENUM_H_1688445072_1719886720

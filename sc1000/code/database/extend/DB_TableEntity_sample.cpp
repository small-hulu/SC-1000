#include "DB_TableEntity_sample.h"

#include <QDebug>

namespace DB {
namespace TableEntity {

QString sample::Get_tableName() const {
    return "sample";
}

QString sample::Get_chineseEnum(int idx) const {
    table_attribute e{idx};
    switch (e) {
    case state             : return "状态标记";
    case isLoad            : return "是否装载";
    case qrcode            : return "条码";
    case project           : return "项目";
    case location          : return "位置";
    case number            : return "编号";
    case repeatCnt         : return "重复次数";
    case detectType        : return "检测类型";
    case sampleTube        : return "样本管";
    case dilutionRatio     : return "稀释倍数";
    case emergency_sign    : return "急诊位";
    case expIdList         : return "实验id列表";
    case expValueList      : return "实验状态列表";
    case remark            : return "备注";

    case patient_name      : return "患者姓名";
    case patient_ageNum    : return "患者年龄";
    case patient_ageUnit   : return "患者年龄的单位";
    case patient_sex       : return "患者性别";
    case patient_patientId : return "患者病人id";
    case patient_department: return "患者科室";
    case patient_bedId     : return "患者床号";
    case patient_inspection: return "患者送检者";
    case patient_verifier  : return "患者检验者";
    case patient_auditor   : return "患者审核者";
    case patient_remark    : return "患者备注";

    case Enum_Count        :;
    default                :;
    }
    return "";
}

const QVector<QString>& sample::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine      = __LINE__;
    enumList[state]              = "state";
    enumList[isLoad]             = "isLoad";
    enumList[qrcode]             = "qrcode";
    enumList[number]             = "number";
    enumList[project]            = "project";
    enumList[location]           = "location";
    enumList[repeatCnt]          = "repeatCnt";
    enumList[detectType]         = "detectType";
    enumList[sampleTube]         = "sampleTube";
    enumList[dilutionRatio]      = "dilutionRatio";
    enumList[emergency_sign]     = "emergency_sign";
    enumList[expIdList]          = "expIdList";
    enumList[expValueList]       = "expValueList";
    enumList[remark]             = "remark";
    enumList[patient_name]       = "patient_name";
    enumList[patient_ageNum]     = "patient_ageNum";
    enumList[patient_ageUnit]    = "patient_ageUnit";
    enumList[patient_sex]        = "patient_sex";
    enumList[patient_patientId]  = "patient_patientId";
    enumList[patient_department] = "patient_department";
    enumList[patient_bedId]      = "patient_bedId";
    enumList[patient_inspection] = "patient_inspection";
    enumList[patient_verifier]   = "patient_verifier";
    enumList[patient_auditor]    = "patient_auditor";
    enumList[patient_remark]     = "patient_remark";
    constexpr int endLine        = __LINE__;

    static_assert(Enum_Count == (endLine - startLine - 1));
    return enumList;
}

}  // namespace TableEntity
}  // namespace DB

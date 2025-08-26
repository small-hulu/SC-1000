#ifndef KEYPROJECT_H_1723618962
#define KEYPROJECT_H_1723618962

#include <QString>

namespace KeyProject {
const QString Version        = "Version";
const QString Time           = "Version";
const QString Title          = "Title";
const QString ProjectMessage = "ProjectMessage";
const QString HelpMessage    = "HelpMessage";
const QString Emergency_t    = "急诊";
const QString convention     = "常规";

const QString Open  = "打开";
const QString Close = "关闭";
const QString Null  = "/";

namespace BaseMsessage {
const QString BaseMsessage     = "BaseMsessage";
const QString ProjectMessage   = "ProjectMessage";
const QString ProjectVersion   = "项目版本号";
const QString ResponsiveMode   = "反应模式";
const QString SampleAspiration = "样本量(μL)";

const QString OneStepMethod        = "一步法";
const QString DelayedOneStepMethod = "延时一步法";

}  // namespace BaseMsessage

namespace FixedDilution {
const QString FixedDilution           = "FixedDilution";
const QString ProjectMessage          = "ProjectMessage";
const QString Fixed_dilution_switch   = "固定稀释开关";
const QString Fixed_dilution_chamber  = "固定稀释腔";
const QString Fixed_dilution_multiple = "固定稀释倍数";
const QString Fixed_dilution_step     = "固定稀释步数";
const QString Fixed_dilution_capacity = "稀释后样本量(μL)";
}  // namespace FixedDilution

namespace WholeBloodPretreatment {}

namespace OtherMessage {
const QString OtherMessage       = "OtherMessage";
const QString Title              = "Title";
const QString ProjectMessage     = "ProjectMessage";
const QString Default_Unit       = "默认单位";
const QString Result_Compensate  = "结果补偿倍数";
const QString Reference_range    = "参考范围";
const QString Result_Calculation = "结果计算";
const QString Result_Explanation = "结果说明";
const QString ProjectName_Ch     = "中文名称";
const QString Single_link        = "单联";
const QString Duplex             = "双联";
const QString Triplet            = "三联";
}  // namespace OtherMessage

namespace ReactionScheme {
const QString R1CLA = "R1组分吸液量(μL)";
const QString R2CLA = "R2组分吸液量(μL)";
const QString R3CLA = "R3组分吸液量(μL)";

const QString R1cavity = "R1组分腔体";
const QString R2cavity = "R2组分腔体";
const QString R3cavity = "R3组分腔体";

const QString ReactionScheme = "ReactionScheme";
const QString ProjectMessage = "ProjectMessage";
const QString HelpMessage    = "HelpMessage";

const QString Step1               = "Step1";
const QString Step2               = "Step2";
const QString TimeStep1           = "第一步孵育时间";
const QString TimeStep2           = "第二步孵育时间";
const QString FinalIncubationTime = "出磁分离孵育时间";
const QString sample_order = "样本添加时机";
}  // namespace ReactionScheme

namespace Pretreatment {}

namespace ReagentMessage {}

}  // namespace KeyProject

#endif  // KEYPROJECT_H_1723618962

#ifndef PWIDGETSAMPLE_H_1724460231
#define PWIDGETSAMPLE_H_1724460231

#include <QJsonArray>
#include <QJsonObject>
#include <QPrintPreviewWidget>
#include <QWidget>

#include "entity/EConfig_Printer.h"
#include "global/GLOBAL_Setting.h"

namespace ST = Setting;
namespace PRINTER {
namespace KeySample {
const QString PrinterList = "PrinterList";

const QString SequenceNumber         = "SequenceNumber";          // 序号
const QString Number                 = "Number";                  // 编号
const QString ProjectName            = "ProjectName";             // 项目名称
const QString Unit                   = "Unit";                    // 单位
const QString Result                 = "Result";                  // 结果
const QString Luminous               = "Luminous";                // 发光值
const QString ReagentBatchNumber     = "ReagentBatchNumber";      // 试剂批号
const QString SubstrateBatchNumber   = "SubstrateBatchNumber";    // 底物批号
const QString CalibrationBatchNumber = "CalibrationBatchNumber";  // 校准品批号
const QString ApplyTime              = "ApplyTime";               // 申请时间
const QString FinishTime             = "FinishTime";              // 完成时间
const QString CalibrationTime        = "CalibrationTime";         // 校准时间
}  // namespace KeySample

namespace KeySample_ts {
const QString PrinterList_ts = "PrinterList_ts";

const QString patient_num        = "patient_num";         // 患者编号
const QString patient_name       = "patient_name";        // 编号
const QString patient_age        = "patient_age";         // 编号
const QString patient_sex        = "patient_sex";         // 患者性别
const QString ProjectName        = "ProjectName";         // 项目名称
const QString ReagentBatchNumber = "ReagentBatchNumber";  // 试剂批号
const QString Result             = "Result";              // 结果
const QString Reference_range    = "Reference_range";     // 参考范围
const QString FinishTime         = "FinishTime";          // 检测时间
}  // namespace KeySample_ts

namespace KeySample_history {
const QString PrinterList_history = "PrinterList_history";

const QString patient_patientId          = "patient_patientId";           // 患者编号
const QString patient_name               = "patient_name";                // 编号
const QString patient_age                = "patient_age";                 // 年龄
const QString patient_ageUnit            = "patient_ageUnit";             // 年龄的单位
const QString patient_sex                = "patient_sex";                 // 患者性别
const QString patient_department         = "patient_department";          // 送检科室
const QString patient_bedId              = "patient_bedId";               // 床号
const QString patient_type               = "patient_type";                // 样本类型
const QString ProjectName                = "ProjectName";                 // 项目名称
const QString ProjectName_Ch             = "ProjectName_Ch";              // 项目中文名称
const QString ReagentBatchNumber         = "ReagentBatchNumber";          // 试剂批号
const QString Result                     = "Result";                      // 结果
const QString Unit                       = "Unit";                        // 单位
const QString Reference_range            = "Reference_range";             // 参考范围
const QString timeStart                  = "timeStart";                   // 开始时间
const QString FinishTime                 = "FinishTime";                  // 检测时间
const QString Result_explanation         = "Result_explanation";          // 检测结果说明
const QString patient_inspection         = "patient_inspection";          // 送检者
const QString patient_verifier           = "patient_verifier";            // 患者检验者
const QString patient_auditor            = "patient_auditor";             // 患者审核者
const QString patient_remark             = "patient_remark";              // 备注
}  // namespace KeySample_history

class PrinterSample {

    using ProjectKey   = QPair<int, QString>;
    using ProjectData  = QStringList;
    using ProjectMap   = QMap<ProjectKey, ProjectData>;
private:
    static void painter_draw(QPaintDevice *device, double res, const QJsonObject &json);
    static void painter_draw_ts(QPaintDevice *device, double res, const QJsonObject &json);  // 热敏
    static void painter_draw_history(QPaintDevice *device, double res, const QJsonObject &json);
    QJsonObject temp_json;  // 暂存预览数据
public:
    static bool                 Exec_print(const QJsonObject &json);
    static QPrintPreviewWidget *Create_widget(const QJsonObject &json, double res,
                                              QWidget *parent = nullptr);
    static QWidget *Create_widget_setting_preview(QString type, double res, const QJsonObject &json,
                                                  QWidget *parent = nullptr);

public slots:
    void Exec_print_signal(QJsonObject json);
};
}  // namespace PRINTER

#endif  // PWIDGETSAMPLE_H_1724460231

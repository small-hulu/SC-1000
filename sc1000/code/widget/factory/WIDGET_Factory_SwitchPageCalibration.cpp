#include "WIDGET_Factory_SwitchPageCalibration.h"

#include "widget/window/calibration/WIDGET_CalibrationApply.h"
#include "widget/window/calibration/WIDGET_CalibrationCurve.h"
#include "widget/window/calibration/WIDGET_CalibrationManage.h"
#include "widget/window/calibration/WIDGET_CalibrationResult.h"
#include "widget/window/calibration/apply/WIDGET_CalibrationApplyList.h"
namespace WIDGET {

namespace {
enum ESubPage_Calibration {
    ESubPage_Calibration_Apply,
    ESubPage_Calibration_Detai,
    ESubPage_Calibration_Result,
    ESubPage_Calibration_Manage,
    ESubPage_Calibration_Curve,
    EnumCount
};
}

QVector<QString> FSwitchPageCalibration::Get_subNameList() {
    QVector<QString> names(EnumCount);
    names[ESubPage_Calibration_Apply]  = "申请";
    names[ESubPage_Calibration_Detai]  = "申请列表";
    names[ESubPage_Calibration_Result] = "结果";
    names[ESubPage_Calibration_Manage] = "校准管理";
    names[ESubPage_Calibration_Curve] = "校准曲线";
    return names;
}

QWidget *FSwitchPageCalibration::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < EnumCount);

    switch (static_cast<ESubPage_Calibration>(index)) {
        using namespace CALIBRATION;
    case ESubPage_Calibration_Apply : return new CalibrationApply(parent);
    case ESubPage_Calibration_Detai : return new CalibrationApplyList(parent);
    case ESubPage_Calibration_Result: return new CalibrationResult(parent);
    case ESubPage_Calibration_Manage: return new CalibrationManage(parent);
    case ESubPage_Calibration_Curve: return new CalibrationCurve(parent);
    case EnumCount                  :;
    }

    return nullptr;
}

}  // namespace WIDGET

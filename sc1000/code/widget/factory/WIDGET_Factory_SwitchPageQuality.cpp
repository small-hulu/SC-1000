#include "WIDGET_Factory_SwitchPageQuality.h"

#include "widget/window/quality/WIDGET_QualityAnalyse.h"
#include "widget/window/quality/WIDGET_QualityApply.h"
#include "widget/window/quality/WIDGET_QualityCurve.h"
#include "widget/window/quality/WIDGET_QualityManage.h"
#include "widget/window/quality/WIDGET_QualityResult.h"
#include "widget/window/quality/apply/WQuality_QualityApplyList.h"

namespace WIDGET {
namespace {
/**
 * 初版
 * 只考虑申请，结果，质控
 */
enum ESubPage_Quality {
    ESubPage_Quality_Apply,      // 申请
    ESubPage_Quality_ApplyList,  // 申请列表
    ESubPage_Quality_Result,     // 结果
    ESubPage_Quality_Analyse,    // 分析
    ESubPage_Quality_Curve,      // 质控图
    ESubPage_Quality_Manage,     // 质控管理

    EnumCount
};
}  // namespace

QVector<QString> FSwitchPageQuality::Get_subNameList() {
    QVector<QString> names(EnumCount);
    names[ESubPage_Quality_Apply]     = "申请";
    names[ESubPage_Quality_ApplyList] = "申请列表";
    names[ESubPage_Quality_Result]    = "结果";
    names[ESubPage_Quality_Analyse]   = "分析";
    names[ESubPage_Quality_Curve]     = "质控图";
    names[ESubPage_Quality_Manage]    = "质控管理";
    return names;
}

QWidget *FSwitchPageQuality::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < EnumCount);

    switch (static_cast<ESubPage_Quality>(index)) {
        using namespace QUALITY;
    case ESubPage_Quality_Apply    : return new QualityApply(parent);
    case ESubPage_Quality_ApplyList: return new QualityApplyList(parent);
    case ESubPage_Quality_Result   : return new QualityResult(parent);
    case ESubPage_Quality_Analyse  : return new QualityAnalyse(parent);
    case ESubPage_Quality_Curve    : return new QualityCurve(parent);
    case ESubPage_Quality_Manage   : return new QualityManage(parent);
    case EnumCount                 :;
    }

    return nullptr;
}

}  // namespace WIDGET

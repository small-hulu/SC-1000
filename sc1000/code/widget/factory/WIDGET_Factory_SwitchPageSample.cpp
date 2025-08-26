#include "WIDGET_Factory_SwitchPageSample.h"

#include "widget/window/sample/WIDGET_SampleApply.h"
#include "widget/window/sample/WIDGET_SampleApplyTable.h"
#include "widget/window/sample/WIDGET_SampleState.h"
#include "widget/window/testui.h"
namespace WIDGET {

namespace {

/**
 * 状态展示功能先延后
 * 有余量时间，再做出来
 */
enum ESubPage_Sample {
    ESubPage_Sample_Apply,   // 申请
    ESubPage_Sample_Detail,  // 申请列表
    ESubPage_Sample_State,   // 状态

    Enum_Count
};
}  // namespace

QVector<QString> FSwitchPageSample::Get_subNameList() {
    QVector<QString> names(Enum_Count);
    names[ESubPage_Sample_Apply]  = "申请";
    names[ESubPage_Sample_Detail] = "申请列表";
    names[ESubPage_Sample_State]  = "状态";
    return names;
}

QWidget *FSwitchPageSample::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < Enum_Count);

    switch (static_cast<ESubPage_Sample>(index)) {
        using namespace SAMPLE;
    case ESubPage_Sample_Apply : return new SampleApply(parent);
    case ESubPage_Sample_Detail: return new SampleApplyTable(parent);
    case ESubPage_Sample_State : return new SampleState(parent);
    case Enum_Count            :;
    }

    return nullptr;
}

}  // namespace WIDGET

#include "WIDGET_Factory_SwitchPageMaintain.h"

#include "widget/window/maintain/WIDGET_MaintainCommonlyUsed.h"
#include "widget/window/maintain/WIDGET_MaintainPower.h"

namespace WIDGET {

namespace {
enum ESubPage {
    ESubPage_Maintain_CommonlyUsed,  // 常用
    ESubPage_Maintain_Power,         // 开关机维护

    EnumCount
};
}  // namespace

QVector<QString> FSwitchPageMaintain::Get_subNameList() {
    QVector<QString> names(EnumCount);
    names[ESubPage_Maintain_CommonlyUsed] = "常用";
    names[ESubPage_Maintain_Power]        = "开关机维护";
    return names;
}

QWidget *FSwitchPageMaintain::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < EnumCount);

    switch (static_cast<ESubPage>(index)) {
        using namespace MAINTAIN;
    case ESubPage_Maintain_CommonlyUsed: return new CommonlyUsed(parent);
    case ESubPage_Maintain_Power       : return new MaintainPower(parent);
    case EnumCount                     :;
    }

    return nullptr;
}

}  // namespace WIDGET

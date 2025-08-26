#include "WIDGET_Factory_SwitchPageReagent.h"

#include "widget/window/reagent/WIDGET_ReagentConsume.h"
#include "widget/window/reagent/WIDGET_ReagentReagent.h"

namespace WIDGET {

namespace {
enum ESubPage_Reagent {
    ENUM_Reagent,  // 试剂（试剂的主界面）
    ENUM_Consume,  // 耗材

    ENUM_Count
};
}

QVector<QString> FSwitchPageReagent::Get_subNameList() {
    QVector<QString> names(ENUM_Count);
    names[ENUM_Reagent] = "试剂";
    names[ENUM_Consume] = "耗材";
    return names;
}

QWidget *FSwitchPageReagent::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < ENUM_Count);

    switch (static_cast<ESubPage_Reagent>(index)) {
        using namespace REAGENT;
    case ENUM_Reagent: return new ReagentReagent(parent);
    case ENUM_Consume: return new ReagentConsume(parent);
    case ENUM_Count  :;
    }

    return nullptr;
}
}  // namespace WIDGET

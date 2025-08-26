#include "WIDGET_Factory_SwitchPageHelp.h"

#include "widget/window/help/WIDGET_HelpAboutDevice.h"
#include "widget/window/help/WIDGET_HelpSpecification.h"
#include "widget/window/help/WIDGET_HelpUpgrade.h"

namespace WIDGET {
namespace {
enum ESubPage : int {
    Page_AboutDevice,
    Page_Upgrade,

    EnumCount
};
}

QVector<QString> FSwitchPageHelp::Get_subNameList() {
    QVector<QString> names(EnumCount);
    names[Page_AboutDevice] = "关于设备";
    names[Page_Upgrade]     = "升级";
    return names;
}

QWidget *FSwitchPageHelp::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < EnumCount);

    switch (static_cast<ESubPage>(index)) {
        using namespace HELP;
    case Page_AboutDevice: return new AboutDevice(parent);
    case Page_Upgrade    : return new HelpUpgrade(parent);
    case EnumCount       :;
    }

    return nullptr;
}

}  // namespace WIDGET

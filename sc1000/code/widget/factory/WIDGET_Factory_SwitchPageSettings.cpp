#include "WIDGET_Factory_SwitchPageSettings.h"

#include "widget/window/setting/WIDGET_SettingDeviceConfig.h"
#include "widget/window/setting/WIDGET_SettingHospitalManage.h"
#include "widget/window/setting/WIDGET_SettingLISManage.h"
#include "widget/window/setting/WIDGET_SettingLogManage.h"
#include "widget/window/setting/WIDGET_SettingPrintManage.h"
#include "widget/window/setting/WIDGET_SettingProjectManage.h"
#include "widget/window/setting/WIDGET_SettingUserManage.h"

namespace WIDGET {

namespace {
enum ESubPage : int {
    Page_ProjectManage,   // 项目管理
    Page_HospitalManage,  // 医院管理
    Page_UserManage,      // 用户管理
    Page_LogManage,       // 日志管理
    Page_PrintManage,     // 打印管理
    Page_LISManage,       // LIS管理
    Page_DeviceConfig,    // 设备配置

    EnumCount
};
}

QVector<QString> FSwitchPageSettings::Get_subNameList() {
    QVector<QString> names(EnumCount);
    names[Page_ProjectManage]  = "项目管理";
    names[Page_HospitalManage] = "医院管理";
    names[Page_UserManage]     = "用户管理";
    names[Page_LogManage]      = "日志管理";
    names[Page_PrintManage]    = "打印管理";
    names[Page_LISManage]      = "LIS管理";
    names[Page_DeviceConfig]   = "设备配置";
    return names;
}

QWidget *FSwitchPageSettings::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < EnumCount);

    switch (static_cast<ESubPage>(index)) {
        using namespace SETTING;
    case Page_ProjectManage : return new ProjectManagement(parent);
    case Page_HospitalManage: return new HospitalManage(parent);
    case Page_UserManage    : return new UserManage(parent);
    case Page_LogManage     : return new LogManage(parent);
    case Page_PrintManage   : return new PrintManage(parent);
    case Page_LISManage     : return new LISManage(parent);
    case Page_DeviceConfig  : return new DeviceConfig(parent);
    case EnumCount          :;
    }

    return nullptr;
}

}  // namespace WIDGET

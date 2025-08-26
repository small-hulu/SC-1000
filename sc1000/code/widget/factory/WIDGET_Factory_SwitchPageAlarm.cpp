#include "WIDGET_Factory_SwitchPageAlarm.h"

#include "widget/window/alarm/WIDGET_AlarmAlarm.h"
#include "widget/window/alarm/WIDGET_LogManage.h"

namespace WIDGET {

namespace {
enum ESubPage_Alarm {
    ESubPage_Alarm_Alarm,
    ESubPage_Alarm_LogManage,

    EnumCount
};
}

QVector<QString> FSwitchPageAlarm::Get_subNameList() {
    QVector<QString> names(EnumCount);
    names[ESubPage_Alarm_Alarm]     = "报警";
    names[ESubPage_Alarm_LogManage] = "日志管理";
    return names;
}

QWidget *FSwitchPageAlarm::Create_bySwitchEnum(int index, QWidget *parent) {
    assert(index < EnumCount);

    switch (static_cast<ESubPage_Alarm>(index)) {
        using namespace ALARM;
    case ESubPage_Alarm_Alarm    : return new AlarmAlarm(parent);
    case ESubPage_Alarm_LogManage: return new LogManage(parent);
    case EnumCount               :;
    }

    return nullptr;
}

}  // namespace WIDGET

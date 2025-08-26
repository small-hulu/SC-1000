#ifndef DEVICESTATE_H_1729067177
#define DEVICESTATE_H_1729067177

#include <QColor>

namespace GLOBAL {
enum class MachineState {
    Normal,   /// 仪器正常
    Running,  /// 仪器运行中
    Waring,   /// 仪器警告
};

QColor Get_DeviceStateColor(MachineState mark);

struct EquipmentRecord {
    static MachineState state;
};

}  // namespace GLOBAL

#endif  // DEVICESTATE_H_1729067177

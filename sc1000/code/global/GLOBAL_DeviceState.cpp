#include "GLOBAL_DeviceState.h"

namespace GLOBAL {

QColor Get_DeviceStateColor(MachineState mark) {
    switch (mark) {
    case MachineState::Normal : return QColor("#0000FF");
    case MachineState::Running: return QColor("#00FF00");
    case MachineState::Waring : return QColor("#FF0000");
    }

    QColor color;
    color.setRgb(255, 255, 255);
    return color;
}

MachineState EquipmentRecord::state = MachineState::Normal;

}  // namespace GLOBAL

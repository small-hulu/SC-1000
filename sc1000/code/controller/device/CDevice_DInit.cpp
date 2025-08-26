#include "CDevice_DInit.h"

#include "CDevice_DLightBand.h"
#include "communication/controller/Init.h"
#include "communication/device/ModbusImpl.h"
#include "communication/utility/ToString.h"
#include "controller/IniSC1000.h"
#include "global/GLOBAL_DeviceState.h"

namespace CONTROLLER::CDevice {

bool DInit::Init() {
    auto& modbus = COM::ModbusImpl::instance();
    if (modbus.isOpen()) {
        qInfo() << "modbus has opened";
        return true;
    }

    bool flag = COM::Init();
    (!flag) && (flag = COM::Init(IniSC1000::instance()[KI::mcu_portName].toString()));
    if (flag) {
        DLightBand::Set_LightBandColor(GLOBAL::Get_DeviceStateColor(GLOBAL::MachineState::Normal));
    }

    return flag;
}

}  // namespace CONTROLLER::CDevice

#include "CDevice_DReagentBin.h"

#include "communication/COM_Device.hpp"
#include "communication/controller/WatchDog.h"
#include "communication/utility/IniConfig.h"

namespace CONTROLLER::CDevice {

using namespace COM;

/**
 * 试剂是一整个容器
 */
bool DReagentBin::ToFront_forReagent(int idx) {
    assert(idx >= 1 && idx <= BinCount);
    idx += -1;

    using Device = COM::DReagentBin;
    auto& device = Device::instance();
    device.Set_Config(Device::Mode_Homing, {});
    device.Exec_sequencedTask();

    auto params = device.GetInfo_frontHole(0);
    device.Offset_byIndex(params, idx);
    device.Offset_byProximity(params);
    device.Set_Config(Device::Mode_Position, params);
    auto ret = device.Exec_sequencedTask();

    return ret == RetCode::Success;
}

/**
 * 样本就对准第一个样本位即可
 * 在[0, 6]中树[0, 3][4, 6]
 */
bool DReagentBin::ToFront_forSample(int idx) {
    assert(idx >= 1 && idx <= BinCount);
    idx += -1;

    using Device = COM::DReagentBin;
    auto& device = Device::instance();
    device.Set_Config(Device::Mode_Homing, {});
    device.Exec_sequencedTask();

    auto params = device.GetInfo_frontHole(6);
    device.Offset_byIndex(params, idx);
    device.Offset_byProximity(params);
    device.Set_Config(Device::Mode_Position, params);
    auto ret = device.Exec_sequencedTask();

    return ret == RetCode::Success;
}

/**
 * @brief DReagentBin::Switch_spinLoop
 * @param open
 * watchdog 需要 reset 后启动才有效
 */
void DReagentBin::Switch_spinLoop(bool open) {
    WatchDog_Guard guard;
    auto&&         ini = IniConfig::instance();
    ini.setValue(KI::enable_reagentBinSpinLoop, (int)open);

    auto&& dog = WatchDog::instance();
    dog.Reset();
}

}  // namespace CONTROLLER::CDevice

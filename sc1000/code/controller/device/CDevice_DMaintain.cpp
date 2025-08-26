#include "CDevice_DMaintain.h"

#include "communication/COM_Device.hpp"
#include "communication/controller/ActionMajor.h"
#include "communication/controller/WatchDog.h"
#include "utility/UTILITY_Sound.h"
namespace CONTROLLER::CDevice {

using namespace COM;

void DMaintain::Homing_allDevice() {
    MultLock::ownership_list ownerList;
    for (int i = 0; i < (int)DeviceManager::atomic_flag::Enum_Count; i += 1) {
        ownerList << &DeviceManager::enum_to_owner((DeviceManager::atomic_flag)i);
    }
    auto lock = MultLock::Create_entity(ownerList);

    auto task = ActionMajor::Major_for_HomingAllDevice();
    task({});
}

void DMaintain::Homing_pump() {
    auto task = ActionMajor::Major_for_HomingAllPump();
    task({});
}

void DMaintain::Maintain_TubleDisCard() {
    auto task = ActionMajor::Major_for_TubleDisCard();
    task({});
}

void DMaintain::Maintain_AbleMagneticSeparationWash() {
    auto task = ActionMajor::Major_for_AbleMagneticSeparationWash();
    task({});
}

void DMaintain::Maintain_AbleASubstrate() {
    auto task = ActionMajor::Major_for_AbleASubstrate();
    task({});
}

void DMaintain::Maintain_AbleBSubstrate() {
    auto task = ActionMajor::Major_for_AbleBSubstrate();
    task({});
}

void DMaintain::Maintain_WashDrain() {
    WatchDog_Guard guard;
    IniConfig::instance().setValue(KI::enable_fillBottle, 0);
    auto task = ActionMajor::Major_for_DrainWash();
    task({});
}

void DMaintain::Maintain_WashFill() {
    auto&& device = DPipetteNeedle::instance();
    auto   param  = device.GetInfo_to_Wash().params;
    device.Set_Config(DPipetteNeedle::Mode_Homing, param);
    device.Exec_sequencedTask();
    device.Set_Config(DPipetteNeedle::Mode_Wash, param);
    device.Exec_sequencedTask();
    device.Set_Config(DPipetteNeedle::Mode_FillBottle, param);
    device.Exec_sequencedTask();
}

void DMaintain::Maintain_WashIntensive() {
    auto task = ActionMajor::Major_for_IntensiveWash();
    task({});
}

}  // namespace CONTROLLER::CDevice

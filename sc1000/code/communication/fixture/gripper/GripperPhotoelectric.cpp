#include "GripperPhotoelectric.h"

#include "communication/device/photoelectric/DPhotoelectric.h"

namespace COM::WFIXTURE {
GripperPhotoelectric::GripperPhotoelectric() : Gripper() {
    auto&& pho = DPhotoelectric::instance();
    pho.Set_Config(DPhotoelectric::Mode::Mode_HomingDoor, {});
    pho.Exec_sequencedTask();
    pho.Set_Config(DPhotoelectric::Mode::Mode_DoorOpen, {});
    pho.Exec_sequencedTask();

    auto&& device      = Device::instance();
    m_params           = device.GetInfo_to_Photoelectric().params;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

GripperPhotoelectric::~GripperPhotoelectric() {
    auto&& device      = Device::instance();
    m_params           = device.Get_mcuParams();
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();

    auto&& pho = DPhotoelectric::instance();
    pho.Set_Config(DPhotoelectric::Mode::Mode_DoorClose, {});
    pho.Exec_sequencedTask();
}

bool GripperPhotoelectric::Save_fixtureParams() {
    auto&& device            = DGripper::instance();
    device.fix.Photoelectric = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
bool GripperPhotoelectric::Save_fixtureParams_Without_Z() {
    auto&& device     = DGripper::instance();
    m_params.zPosition = o_params.zPosition;
    device.fix.Photoelectric = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

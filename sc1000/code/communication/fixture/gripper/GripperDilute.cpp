#include "GripperDilute.h"

namespace COM::WFIXTURE {
GripperDilute::GripperDilute() {
    auto&& device      = Device::instance();
    m_params           = device.GetInfo_to_Dilute().params;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

bool GripperDilute::Save_fixtureParams() {
    auto&& device     = DGripper::instance();
    device.fix.Dilute = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
bool GripperDilute::Save_fixtureParams_Without_Z() {
    auto&& device  = DGripper::instance();
    m_params.zPosition = o_params.zPosition;
    device.fix.Dilute = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

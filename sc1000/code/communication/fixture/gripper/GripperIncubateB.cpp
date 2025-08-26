#include "GripperIncubateB.h"

namespace COM::WFIXTURE {
GripperIncubateB::GripperIncubateB() {
    auto&& device      = Device::instance();
    m_params           = device.fix.IncubateB;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
}

bool GripperIncubateB::Save_fixtureParams() {
    auto&& device        = DGripper::instance();
    device.fix.IncubateB = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
bool GripperIncubateB::Save_fixtureParams_Without_Z() {
    auto&& device        = DGripper::instance();
    device.fix.IncubateB = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

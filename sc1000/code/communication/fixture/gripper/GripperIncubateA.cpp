#include "GripperIncubateA.h"

namespace COM::WFIXTURE {
GripperIncubateA::GripperIncubateA() {
    auto&& device      = Device::instance();
    m_params           = device.fix.IncubateA;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
}

bool GripperIncubateA::Save_fixtureParams() {
    auto&& device        = DGripper::instance();
    device.fix.IncubateA = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}

///在Z不动的情况下仅存XY的位置
bool GripperIncubateA::Save_fixtureParams_Without_Z() {
    auto&& device  = DGripper::instance();
    m_params.zPosition = o_params.zPosition;
    device.fix.IncubateA = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

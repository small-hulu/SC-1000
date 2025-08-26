#include "GripperIncubateC.h"

namespace COM::WFIXTURE {
GripperIncubateC::GripperIncubateC() {
    auto&& device      = Device::instance();
    m_params           = device.fix.IncubateC;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
}

bool GripperIncubateC::Save_fixtureParams() {
    auto&& device        = DGripper::instance();
    device.fix.IncubateC = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
bool GripperIncubateC::Save_fixtureParams_Without_Z() {
    auto&& device        = DGripper::instance();
    device.fix.IncubateC = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

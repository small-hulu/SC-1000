#include "GripperMixing.h"

#include "communication/utility/JsonConfig.h"

namespace COM::WFIXTURE {
GripperMixing::GripperMixing() : Gripper() {
    auto&& device      = Device::instance();
    m_params           = device.GetInfo_to_Mixing().params;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

bool GripperMixing::Save_fixtureParams() {
    auto&& device     = DGripper::instance();
    device.fix.Mixing = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
bool GripperMixing::Save_fixtureParams_Without_Z() {
    auto&& device     = DGripper::instance();
    m_params.zPosition = o_params.zPosition;
    device.fix.Mixing = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

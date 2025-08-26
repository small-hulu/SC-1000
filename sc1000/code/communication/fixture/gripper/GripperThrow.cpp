#include "GripperThrow.h"

#include "communication/utility/JsonConfig.h"

namespace COM::WFIXTURE {
GripperThrow::GripperThrow() : Gripper() {
    auto&& device      = Device::instance();
    m_params           = device.GetInfo_to_Throw().params;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

bool GripperThrow::Save_fixtureParams() {
    auto&& device    = DGripper::instance();
    device.fix.Throw = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}

bool GripperThrow::Save_fixtureParams_Without_Z() {
    auto&& device      = DGripper::instance();
    m_params.zPosition = o_params.zPosition;
    device.fix.Throw   = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}

}  // namespace COM::WFIXTURE

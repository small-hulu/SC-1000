#include "PipetteNeedleDilute.h"

namespace COM::WFIXTURE {
PipetteNeedleDilute::PipetteNeedleDilute() {
    auto&& device      = Device::instance();
    m_params           = device.GetInfo_to_Dilute().params;
    o_params           = device.GetInfo_to_Dilute().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_DebugYZPosition, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

bool PipetteNeedleDilute::Save_fixtureParams() {
    auto&& device               = Device::instance();
    device.fix.Dilute.yPosition = m_params.yPosition;
    device.fix.Dilute.zPosition = m_params.zPosition;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
bool PipetteNeedleDilute::Save_fixtureParams_Without_Z() {
    auto&& device               = Device::instance();
    m_params.zPosition = o_params.zPosition;
    device.fix.Dilute.yPosition = m_params.yPosition;
    device.fix.Dilute.zPosition = m_params.zPosition;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

#include "FixCupRangeMax.h"

namespace COM::WFIXTURE {
FixCupRangeMax::FixCupRangeMax() {
    auto&& device           = DMixing::instance();
    m_params                = device.Get_mcuParams();
    m_params.cupTarPosition = m_params.cupOutPosition;
    device.Set_Config(Device::Mode_DebugCup, m_params);
    device.Exec_sequencedTask();
}

bool FixCupRangeMax::Save_fixtureParams() {
    auto&& device       = DMixing::instance();
    device.fix.rangeMax = m_params.cupTarPosition;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

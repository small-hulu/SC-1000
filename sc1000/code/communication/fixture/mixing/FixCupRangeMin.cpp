#include "FixCupRangeMin.h"

#include "communication/config/CKeyJson.hpp"
#include "communication/utility/JsonConfig.h"

namespace COM::WFIXTURE {
FixCupRangeMin::FixCupRangeMin() {
    auto&& device           = DMixing::instance();
    m_params                = device.Get_mcuParams();
    m_params.cupTarPosition = m_params.cupInPosition;
    device.Set_Config(Device::Mode_DebugCup, m_params);
    device.Exec_sequencedTask();
}

bool FixCupRangeMin::Save_fixtureParams() {
    auto&& device       = DMixing::instance();
    device.fix.rangeMin = m_params.cupTarPosition;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

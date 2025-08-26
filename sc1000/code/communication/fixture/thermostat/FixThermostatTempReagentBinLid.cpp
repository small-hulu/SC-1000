#include "FixThermostatTempReagentBinLid.h"

namespace COM::WFIXTURE {
FixThermostatTempReagentBinLid::FixThermostatTempReagentBinLid() {
    double val = Device::instance().fix.tempReagentBinLid;
    set_inputNum(val);
}

bool FixThermostatTempReagentBinLid::Save_fixtureParams() {
    auto&& device                = Device::instance();
    device.fix.tempReagentBinLid = get_inputNum();
    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

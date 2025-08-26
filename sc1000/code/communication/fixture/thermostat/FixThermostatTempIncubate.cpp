#include "FixThermostatTempIncubate.h"

namespace COM::WFIXTURE {
FixThermostatTempIncubate::FixThermostatTempIncubate() {
    double val = Device::instance().fix.tempIncubate;
    set_inputNum(val);
}

bool FixThermostatTempIncubate::Save_fixtureParams() {
    auto&& device           = Device::instance();
    device.fix.tempIncubate = get_inputNum();
    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

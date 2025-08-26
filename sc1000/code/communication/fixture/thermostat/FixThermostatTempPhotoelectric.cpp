#include "FixThermostatTempPhotoelectric.h"

namespace COM::WFIXTURE {
FixThermostatTempPhotoelectric::FixThermostatTempPhotoelectric() {
    double val = Device::instance().fix.tempPhotoelectric;
    set_inputNum(val);
}

bool FixThermostatTempPhotoelectric::Save_fixtureParams() {
    auto&& device                = Device::instance();
    device.fix.tempPhotoelectric = get_inputNum();
    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

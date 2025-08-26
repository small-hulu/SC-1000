#include "PhotoelectricBCapacity.h"

namespace COM::WFIXTURE {
PhotoelectricBCapacity::PhotoelectricBCapacity() : Photoelectric() {
    const QString deviceName = KJ::Photoelectric;
    QJsonObject   jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject   deviceFix  = jsonObj[deviceName].toObject();
    int           num        = deviceFix[KJ::capacity].toInt();
    set_inputNum(num);
}

bool PhotoelectricBCapacity::Save_fixtureParams() {
    auto&& device       = DPhotoelectric::instance();
    device.fix.capacity = get_inputNum();

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

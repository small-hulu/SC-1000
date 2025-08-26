#include "DPhotoelectric.h"

#include "../../config/CKeyJson.hpp"
#include "../../utility/JsonConfig.h"

namespace COM {

DPhotoelectric::DPhotoelectric(QObject *parent) : APhotoelectric(parent) {
    setObjectName(QString("光电仓"));

    Reset_cacheFixtureParams();
}

void DPhotoelectric::Reset_cacheFixtureParams() {
    auto json    = JsonConfig::instance()[KJ::Photoelectric];
    fix.capacity = json[KJ::capacity].toInt();
    fix.rangeMin = json[KJ::fix][KJ::rangeMin].toInt();
}

void DPhotoelectric::Save_fileFixtureParams() {
    const QString deviceName = KJ::Photoelectric;

    QJsonObject jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject jsonDevice = jsonObj[deviceName].toObject();
    QJsonObject jsonfix    = jsonDevice[KJ::fix].toObject();
    jsonfix[KJ::rangeMin]  = fix.rangeMin;

    jsonDevice[KJ::capacity] = fix.capacity;
    jsonDevice[KJ::fix]      = jsonfix;
    jsonObj[deviceName]      = jsonDevice;

    JsonConfig::instance().Save(jsonObj);
}

RetCode DPhotoelectric::Init_muc() {
    QModbusDataUnit dataUnit = create_modbusData(2, 1006);
    dataUnit.setValue(0, fix.rangeMin);
    dataUnit.setValue(1, fix.rangeMin >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return RetCode::Success;
}

}  // namespace COM

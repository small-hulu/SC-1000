#include "DMixing.h"

#include "../../config/CKeyJson.hpp"
#include "../../utility/JsonConfig.h"
#include "../../utility/PDT.h"

namespace COM {

DMixing::DMixing(QObject *parent) : AMixing(parent) {
    setObjectName(QString("分杯混匀"));

    Reset_cacheFixtureParams();
}

void DMixing::Reset_cacheFixtureParams() {
    auto json    = JsonConfig::instance()[KJ::Mixing];
    fix.rangeMin = json[KJ::Cup][KJ::fix][KJ::rangeMin].toInt();
    fix.rangeMax = json[KJ::Cup][KJ::fix][KJ::rangeMax].toInt();
}

void DMixing::Save_fileFixtureParams() {
    const QString deviceName = KJ::Mixing;

    QJsonObject jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject jsonMixing = jsonObj[KJ::Mixing].toObject();
    QJsonObject jsonCup    = jsonMixing[KJ::Cup].toObject();
    QJsonObject mixingCupFix;
    mixingCupFix[KJ::rangeMin] = fix.rangeMin;
    mixingCupFix[KJ::rangeMax] = fix.rangeMax;

    jsonCup[KJ::fix]    = mixingCupFix;
    jsonMixing[KJ::Cup] = jsonCup;
    jsonObj[deviceName] = jsonMixing;

    JsonConfig::instance().Save(jsonObj);
}

RetCode DMixing::Init_muc() {
    return Init_position(fix.rangeMin, fix.rangeMax);
}

RetCode DMixing::Init_position(int in, int out) {
    QModbusDataUnit dataUnit = create_modbusData(4, 1010);
    uint16_t        low, high;

    std::tie(low, high) = oneu32_to_twou16(in);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    std::tie(low, high) = oneu32_to_twou16(out);
    dataUnit.setValue(2, low);
    dataUnit.setValue(3, high);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return RetCode::Success;
}

bool DMixing::check_cup() {
    Set_Config(DMixing::Mode_EnableDivideCup, m_params);
    Exec_sequencedTask();
    return Get_mcuStateBits().u1000_Mixing.b02_cupholeFull == 1;
}

}  // namespace COM

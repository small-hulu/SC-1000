#include "DThermostat.h"

#include "../../config/CKeyJson.hpp"
#include "../../utility/IniConfig.h"
#include "../../utility/JsonConfig.h"

namespace COM {

DThermostat::DThermostat(QObject *parent) : AThermostat(parent) {
    setObjectName(QString("温控"));

    Reset_cacheFixtureParams();
}

void DThermostat::Reset_cacheFixtureParams() {
    QJsonValue json       = JsonConfig::instance()[KJ::Thermostat];
    fix.tempIncubate      = json[KJ::Incubate][KJ::fix][KJ::temp].toDouble();
    fix.tempPhotoelectric = json[KJ::Photoelectric][KJ::fix][KJ::temp].toDouble();
    fix.tempReagentBinLid = json[KJ::ReagentBinLid][KJ::fix][KJ::temp].toDouble();
}

void DThermostat::Save_fileFixtureParams() {
    const QString deviceName = KJ::Thermostat;

    QJsonObject jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject jsonDevice = jsonObj[deviceName].toObject();

    QJsonObject jsonSub;
    QJsonObject jsonFix;

    jsonSub                  = jsonDevice[KJ::Incubate].toObject();
    jsonFix[KJ::temp]        = fix.tempIncubate;
    jsonSub[KJ::fix]         = jsonFix;
    jsonDevice[KJ::Incubate] = jsonSub;

    jsonSub                       = jsonDevice[KJ::Photoelectric].toObject();
    jsonFix[KJ::temp]             = fix.tempPhotoelectric;
    jsonSub[KJ::fix]              = jsonFix;
    jsonDevice[KJ::Photoelectric] = jsonSub;

    jsonSub                       = jsonDevice[KJ::ReagentBinLid].toObject();
    jsonFix[KJ::temp]             = fix.tempReagentBinLid;
    jsonSub[KJ::fix]              = jsonFix;
    jsonDevice[KJ::ReagentBinLid] = jsonSub;

    jsonObj[deviceName] = jsonDevice;
    JsonConfig::instance().Save(jsonObj);
}

RetCode DThermostat::Init_muc() {
    auto &&ini = IniConfig::instance();

    Params params;

    params.photoelectricAble = true;
    params.photoelectricTemp = fix.tempPhotoelectric + ini[KI::temp_Photoelectric].toDouble();
    Set_Config(Mode_PhotoelectricAble, params);
    Exec_sequencedTask();

    params.incubateAble = true;
    params.incubateTemp = fix.tempIncubate + ini[KI::temp_Incubate].toDouble();
    Set_Config(Mode_IncubateAble, params);
    Exec_sequencedTask();

    params.reagentBinLidAble = true;
    params.reagentBinLidTemp = fix.tempReagentBinLid + ini[KI::temp_ReagentBinLid].toDouble();
    Set_Config(Mode_ReagentBinLidAble, params);
    Exec_sequencedTask();

    params.reagentBinAble = true;
    Set_Config(Mode_ReagentBinAble, params);
    Exec_sequencedTask();

    params.fan0Able = true;
    Set_Config(Mode_Fan0Able, params);
    Exec_sequencedTask();
    params.fan1Able = true;
    Set_Config(Mode_Fan1Able, params);
    Exec_sequencedTask();
    params.fan2Able = true;
    Set_Config(Mode_Fan2Able, params);
    Exec_sequencedTask();
    params.fan3Able = true;
    Set_Config(Mode_Fan3Able, params);
    Exec_sequencedTask();

    return RetCode::Success;
}

DThermostat::Params DThermostat::Get_MCUTemp() {
    auto   param             = Get_mcuParams();
    auto &&ini               = IniConfig::instance();
    param.photoelectricTemp -= ini[KI::temp_Photoelectric].toDouble();
    param.incubateTemp      -= ini[KI::temp_Incubate].toDouble();
    param.reagentBinLidTemp -= ini[KI::temp_ReagentBinLid].toDouble();
    return param;
}

}  // namespace COM

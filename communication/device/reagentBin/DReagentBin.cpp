#include "DReagentBin.h"

#include <QColor>

#include "../../config/CKeyJson.hpp"
#include "../../utility/Helper.h"
#include "../../utility/JsonConfig.h"

namespace COM {

DReagentBin::DReagentBin(QObject *parent) : AReagentBin(parent) {
    setObjectName(QString("试剂仓"));

    Reset_cacheFixtureParams();
}

void DReagentBin::Reset_cacheFixtureParams() {
    QJsonValue json = JsonConfig::instance()[KJ::ReagentBin];
    fix.offset      = json[KJ::offset].toInt();
    fix.count       = json[KJ::count].toInt();

    QJsonArray posList = json[KJ::idxBinBack].toArray();
    fix.backList.resize(posList.size());
    for (int i = 0; i < posList.size(); i += 1) {
        fix.backList[i] = posList[i].toInt();
    }

    posList = json[KJ::idxBinFront].toArray();
    fix.frontList.resize(posList.size());
    for (int i = 0; i < posList.size(); i += 1) {
        fix.frontList[i] = posList[i].toInt();
    }
}

void DReagentBin::Save_fileFixtureParams() {
    const QString deviceName = KJ::ReagentBin;

    QJsonObject jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject jsonDevice = jsonObj[deviceName].toObject();

    jsonDevice[KJ::count]  = fix.count;
    jsonDevice[KJ::offset] = fix.offset;

    QJsonArray arr;
    for (auto num : fix.backList) {
        arr << num;
    }
    jsonDevice[KJ::idxBinBack] = arr;

    arr = QJsonArray();
    for (auto num : fix.frontList) {
        arr << num;
    }
    jsonDevice[KJ::idxBinFront] = arr;

    jsonObj[deviceName] = jsonDevice;
    JsonConfig::instance().Save(jsonObj);
}

RetCode DReagentBin::Init_muc() {
    qCritical() << objectName() << __func__ << " empty func ";
    return RetCode::TODO;
}

DReagentBin::Params DReagentBin::Offset_byProximity(IDeviceConfig::Params &params) const {
    const int curLocal    = Get_mcuParams().position;
    const int targetLocal = params.position;
    const int cycle       = fix.offset * fix.count;
    params.position       = Helper::offset_byProximity(curLocal, targetLocal, cycle);
    return params;
}

void DReagentBin::Set_LED_RGB(int R, int G, int B) {
    QModbusDataUnit dataUnit = create_modbusData(3, 1014);
    dataUnit.setValue(0, R);
    dataUnit.setValue(1, G);
    dataUnit.setValue(2, B);
    auto reply = write(dataUnit);

    qInfo() << "Set RGB =" << QColor(R, G, B) << reply.is_valid();
}

void DReagentBin::Loop_self() {
    const int offset = Get_periodOfRotation();
    auto      params = Params{};

    params.position = 0;
    Set_Config(Mode_Position, params);
    Exec_sequencedTask();

    for ([[maybe_unused]] int _ : {1, 2, 3}) {
        params.position = 1.0 * offset * 0.33;
        Set_Config(Mode_Position, params);
        Exec_sequencedTask();

        params.position = -1.0 * offset * 0.33;
        Set_Config(Mode_Position, params);
        Exec_sequencedTask();
    }

    Set_Config(Mode_Homing, params);
    Exec_sequencedTask();
}

}  // namespace COM

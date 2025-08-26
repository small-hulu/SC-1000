#include "DMagneticSeparation.h"

#include "../../config/CKeyJson.hpp"
#include "../../utility/Helper.h"
#include "../../utility/JsonConfig.h"

namespace COM {

/**
 * constructor
 * [1] 标记 QObject 的信息
 * [2] 初始化table
 * [3] 初始化孔位的偏移量
 *     - 从配置中读取
 */
DMagneticSeparation::DMagneticSeparation(QObject *parent) : AMagneticSeparation(parent) {
    setObjectName(QString("磁分离"));

    Reset_cacheFixtureParams();
    {
        m_fixTable[KJ::Gripper]    = std::bind(&DMagneticSeparation::GetParams_to_Grapper, this);
        m_fixTable[KJ::Wash]       = std::bind(&DMagneticSeparation::GetParams_to_Wash, this);
        m_fixTable[KJ::Needle0]    = std::bind(&DMagneticSeparation::GetParams_to_Needle0, this);
        m_fixTable[KJ::Needle1]    = std::bind(&DMagneticSeparation::GetParams_to_Needle1, this);
        m_fixTable[KJ::Needle2]    = std::bind(&DMagneticSeparation::GetParams_to_Needle2, this);
        m_fixTable[KJ::ASubstrate] = std::bind(&DMagneticSeparation::GetParams_to_ASubstrate, this);
    }
}

/**
 * 务必包含base的to_string
 */
QString DMagneticSeparation::to_string() const {
    QString msg;
    msg += AMagneticSeparation::to_string();
    msg += m_cu.to_string();
    return msg;
}
/*"MagneticSeparation": {
        "ASubstrate": 14040,
        "Gripper": -360,
        "Needle0": 12440,
        "Needle1": 9240,
        "Needle2": 4440,
        "Wash": 2840,
        "aSubstrateCapacity": 12800,
        "fix": {
            "count": 12,
            "drainDeep": 34000,
            "mixHeight": -7000,
            "offset": -1600
        },
        "message": "Capacity 是默认期望值，可在具体的json中覆盖",
        "washCapacity": 10000
    }
*/
void DMagneticSeparation::Reset_cacheFixtureParams() {
    auto json              = JsonConfig::instance()[KJ::MagneticSeparation];
    fix.offset             = json[KJ::fix][KJ::offset].toDouble();
    fix.count              = json[KJ::fix][KJ::count].toDouble();
    fix.mixHeight          = json[KJ::fix][KJ::mixHeight].toDouble();
    fix.drainDeep          = json[KJ::fix][KJ::drainDeep].toDouble();
    fix.Gripper            = json[KJ::Gripper].toDouble();
    fix.Wash               = json[KJ::Wash].toDouble();
    fix.Needle0            = json[KJ::Needle0].toDouble();
    fix.Needle1            = json[KJ::Needle1].toDouble();
    fix.Needle2            = json[KJ::Needle2].toDouble();
    fix.ASubstrate         = json[KJ::ASubstrate].toDouble();
    fix.washCapacity       = json[KJ::washCapacity].toDouble();
    fix.aSubstrateCapacity = json[KJ::aSubstrateCapacity].toDouble();
    assert(MCell_Count == fix.count);
}

void DMagneticSeparation::Save_fileFixtureParams() {
    const QString deviceName = KJ::MagneticSeparation;

    QJsonObject jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject jsonDevice = jsonObj[deviceName].toObject();
    QJsonObject jsonfix    = jsonDevice[KJ::fix].toObject();
    jsonfix[KJ::count]     = fix.count;
    jsonfix[KJ::drainDeep] = fix.drainDeep;
    jsonfix[KJ::mixHeight] = fix.mixHeight;
    jsonfix[KJ::offset]    = fix.offset;
    jsonDevice[KJ::fix]    = jsonfix;

    jsonDevice[KJ::Gripper]            = fix.Gripper;
    jsonDevice[KJ::Wash]               = fix.Wash;
    jsonDevice[KJ::Needle0]            = fix.Needle0;
    jsonDevice[KJ::Needle1]            = fix.Needle1;
    jsonDevice[KJ::Needle2]            = fix.Needle2;
    jsonDevice[KJ::ASubstrate]         = fix.ASubstrate;
    jsonDevice[KJ::aSubstrateCapacity] = fix.aSubstrateCapacity;
    jsonDevice[KJ::washCapacity]       = fix.washCapacity;
    jsonObj[deviceName]                = jsonDevice;

    JsonConfig::instance().Save(jsonObj);
}

RetCode DMagneticSeparation::Init_muc() {
    QModbusDataUnit dataUnit = create_modbusData(4, 1012);
    dataUnit.setValue(0, fix.drainDeep);
    dataUnit.setValue(1, fix.drainDeep >> 16u);
    dataUnit.setValue(2, fix.mixHeight);
    dataUnit.setValue(3, fix.mixHeight >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return RetCode::Success;
}

void DMagneticSeparation::Calc_byGripper(int pos) {
    int offset     = std::abs(fix.offset);
    fix.Gripper    = pos;
    fix.Wash       = pos + 2 * offset;
    fix.Needle0    = pos + 8 * offset;
    fix.Needle1    = pos + 6 * offset;
    fix.Needle2    = pos + 3 * offset;
    fix.ASubstrate = pos + 9 * offset;
}

DMagneticSeparation::Params DMagneticSeparation::Offset_byProximity(
    IDeviceConfig::Params &params) const {
    const int curLocal    = Get_mcuParams().position;
    const int targetLocal = params.position;
    const int cycle       = fix.offset * fix.count;
    params.position       = Helper::offset_byProximity(curLocal, targetLocal, cycle);
    return params;
}

DMagneticSeparation::Params DMagneticSeparation::Offset_byIndex(DMagneticSeparation::Params &params,
                                                                int idx) const {
    assert(idx < MCell_Count);
    params.position += idx * fix.offset;
    return params;
}

DMagneticSeparation::Params DMagneticSeparation::Get_fixParams(const QString &name) const {
    if (false) {
    } else if (name.isEmpty()) {
        return {};
    } else if (m_fixTable.count(name)) {
        return m_fixTable[name]();
    } else {
        assert(false && "磁分离table解析异常键值");
        return {};
    }
}

}  // namespace COM

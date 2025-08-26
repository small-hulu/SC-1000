#include "DGripper.h"

#include <QDebug>
#include <QHash>
#include <functional>

#include "../../config/CKeyJson.hpp"
#include "../../controller/Event.h"
#include "../../utility/Helper.h"
#include "../../utility/JsonConfig.h"
#include "../../utility/PDT.h"
#include "../../utility/ToString.h"
#include "../incubate/DIncubate.h"

namespace COM {

DGripper::DGripper(QObject* parent) : AGripper(parent) {
    setObjectName(QString("夹爪"));

    Reset_cacheFixtureParams();
    /// fix-table
    {
        //! KJ::Incubate 函数的解析方式是有参数模式的
        //! 因此这里不注册，在获取解析table时再做特殊处理
        m_fixTable[KJ::Cup]    = std::bind(&DGripper::GetInfo_to_Cup, this);
        m_fixTable[KJ::Mixing] = std::bind(&DGripper::GetInfo_to_Mixing, this);
        m_fixTable[KJ::MagneticSeparation] =
            std::bind(&DGripper::GetInfo_to_MagneticSeparation, this);
        m_fixTable[KJ::Photoelectric] = std::bind(&DGripper::GetInfo_to_Photoelectric, this);
        m_fixTable[KJ::Throw]         = std::bind(&DGripper::GetInfo_to_Throw, this);
        m_fixTable[KJ::Load]          = std::bind(&DGripper::GetInfo_to_Load, this);
        m_fixTable[KJ::Dilute]        = std::bind(&DGripper::GetInfo_to_Dilute, this);
    }
}

void DGripper::Reset_cacheFixtureParams() {
    /// 张开角度不用管
    auto params_copy = [](Params& params, const QJsonValue json) -> void {
        params.isValid     = true;
        params.xPosition   = json[KJ::x].toInt();
        params.yPosition   = json[KJ::y].toInt();
        params.zPosition   = json[KJ::z].toInt();
        params.deepGrab    = params.zPosition;
        params.deepRelease = params.zPosition;
    };
    auto json = JsonConfig::instance()[KJ::Gripper];

    params_copy(fix.Cup, json[KJ::Cup]);
    params_copy(fix.Mixing, json[KJ::Mixing]);
    params_copy(fix.MagneticSeparation, json[KJ::MagneticSeparation]);
    params_copy(fix.Photoelectric, json[KJ::Photoelectric]);
    params_copy(fix.Throw, json[KJ::Throw]);
    params_copy(fix.Load, json[KJ::Load]);
    params_copy(fix.Dilute, json[KJ::Dilute]);
    auto incubateList = json[KJ::Incubate].toArray();
    for (auto&& item : incubateList) {
        QJsonObject obj              = item.toObject();
        int         i                = obj[KJ::i].toInt();
        int         j                = obj[KJ::j].toInt();
        fix.Incubate[i][j].xPosition = obj[KJ::x].toInt();
        fix.Incubate[i][j].yPosition = obj[KJ::y].toInt();
        fix.Incubate[i][j].isValid   = true;
    }

    fix.rangeMin        = json[KJ::fix][KJ::rangeMin].toInt();
    fix.rangeMax        = json[KJ::fix][KJ::rangeMax].toInt();
    fix.zPositionHeight = json[KJ::fix][KJ::zPositionHeight].toInt();

    Helper::CalcHoles holes;
    fix.IncubateA         = GetInfo_to_Incubate_byXY(holes.a.i, holes.a.j).params;
    fix.IncubateB         = GetInfo_to_Incubate_byXY(holes.b.i, holes.b.j).params;
    fix.IncubateC         = GetInfo_to_Incubate_byXY(holes.c.i, holes.c.j).params;
    fix.IncubateA.isValid = true;
    fix.IncubateB.isValid = true;
    fix.IncubateC.isValid = true;

    DIncubate::instance().Get_CU().Reset_cacheFixtureParams();
}

void DGripper::Save_fileFixtureParams() {
    const QString deviceName = KJ::Gripper;

    QJsonObject jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject jsonDevice = jsonObj[deviceName].toObject();

    /// [夹爪张开极限]
    QJsonObject jsonSub;
    jsonSub[KJ::rangeMin]        = fix.rangeMin;
    jsonSub[KJ::rangeMax]        = fix.rangeMax;
    jsonSub[KJ::zPositionHeight] = fix.zPositionHeight;
    jsonDevice[KJ::fix]          = jsonSub;

    auto params_to_fix = [&jsonDevice](QString key, const Params& params) -> void {
        QJsonObject json;
        json[KJ::x]     = params.xPosition;
        json[KJ::y]     = params.yPosition;
        json[KJ::z]     = params.zPosition;
        jsonDevice[key] = json;
    };

    /// [各个单一孔位的固定位置]
    params_to_fix(KJ::Cup, fix.Cup);
    params_to_fix(KJ::Mixing, fix.Mixing);
    params_to_fix(KJ::MagneticSeparation, fix.MagneticSeparation);
    params_to_fix(KJ::Photoelectric, fix.Photoelectric);
    params_to_fix(KJ::Throw, fix.Throw);
    params_to_fix(KJ::Load, fix.Load);
    params_to_fix(KJ::Dilute, fix.Dilute);

    /// 温裕盘
    Helper::CalcHoles holes;
    holes.a.x           = fix.IncubateA.xPosition;
    holes.a.y           = fix.IncubateA.yPosition;
    holes.b.x           = fix.IncubateB.xPosition;
    holes.b.y           = fix.IncubateB.yPosition;
    holes.c.x           = fix.IncubateC.xPosition;
    holes.c.y           = fix.IncubateC.yPosition;
    auto       holeList = holes.get_pointList();
    QJsonArray holeArr;
    for (auto point : holeList) {
        holeArr << point.to_json();
    }
    jsonDevice[KJ::Incubate] = holeArr;

    jsonObj[deviceName] = jsonDevice;
    JsonConfig::instance().Save(jsonObj);
}

void DGripper::Save_fileFixtureParams_Without_Z() {
    const QString deviceName = KJ::Gripper;

    QJsonObject jsonObj    = JsonConfig::instance().Get_json();
    QJsonObject jsonDevice = jsonObj[deviceName].toObject();

    /// [夹爪张开极限]
    QJsonObject jsonSub;
    jsonSub[KJ::rangeMin]        = fix.rangeMin;
    jsonSub[KJ::rangeMax]        = fix.rangeMax;
    jsonSub[KJ::zPositionHeight] = fix.zPositionHeight;
    jsonDevice[KJ::fix]          = jsonSub;

    auto params_to_fix = [&jsonDevice](QString key, const Params& params) -> void {
        QJsonObject json;
        json[KJ::x]     = params.xPosition;
        json[KJ::y]     = params.yPosition;
        //json[KJ::z]     = params.zPosition;
        jsonDevice[key] = json;
    };

    /// [各个单一孔位的固定位置]
    params_to_fix(KJ::Cup, fix.Cup);
    params_to_fix(KJ::Mixing, fix.Mixing);
    params_to_fix(KJ::MagneticSeparation, fix.MagneticSeparation);
    params_to_fix(KJ::Photoelectric, fix.Photoelectric);
    params_to_fix(KJ::Throw, fix.Throw);
    params_to_fix(KJ::Load, fix.Load);
    params_to_fix(KJ::Dilute, fix.Dilute);

    /// 温裕盘
    Helper::CalcHoles holes;
    holes.a.x           = fix.IncubateA.xPosition;
    holes.a.y           = fix.IncubateA.yPosition;
    holes.b.x           = fix.IncubateB.xPosition;
    holes.b.y           = fix.IncubateB.yPosition;
    holes.c.x           = fix.IncubateC.xPosition;
    holes.c.y           = fix.IncubateC.yPosition;
    auto       holeList = holes.get_pointList();
    QJsonArray holeArr;
    for (auto point : holeList) {
        holeArr << point.to_json();
    }
    jsonDevice[KJ::Incubate] = holeArr;

    jsonObj[deviceName] = jsonDevice;
    JsonConfig::instance().Save(jsonObj);
}

RetCode DGripper::Init_muc() {
    QModbusDataUnit dataUnit = create_modbusData(2, 1028);
    uint16_t        low, high;
    std::tie(low, high) = oneu32_to_twou16(fix.zPositionHeight);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    if (!write(dataUnit).is_valid()) {
        qWarning() << "modbus write error";
    }

    Set_ZPositionHeightFlag(false);
    return Set_rangeDefault(fix.rangeMin, fix.rangeMax);
}

RetCode DGripper::Set_rangeDefault(int32_t valClose, int32_t valOpen) {
    QModbusDataUnit dataUnit = create_modbusData(4, 1024);
    uint16_t        low, high;

    std::tie(low, high) = oneu32_to_twou16(valClose);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    std::tie(low, high) = oneu32_to_twou16(valOpen);
    dataUnit.setValue(2, low);
    dataUnit.setValue(3, high);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return RetCode::Success;
}

DGripper::InfoPkg DGripper::Get_fixInfo(const QString& name) const {
    if (false) {
    } else if (name.isEmpty()) {
        return {};
    } else if (name == KJ::Incubate) {
        return {};
    } else if (m_fixTable.count(name)) {
        return m_fixTable[name]();
    } else {
        assert(false && "夹爪table解析异常键值");
        return {};
    }
}

/**
 * 分杯功能在混匀电机中
 */
DGripper::InfoPkg DGripper::GetInfo_to_Cup() const {
    InfoPkg info;
    info.params = fix.Cup;
    return info;
}

/**
 * 注意混匀板不止混匀电机一个
 */
DGripper::InfoPkg DGripper::GetInfo_to_Mixing() const {
    InfoPkg info;
    info.params = fix.Mixing;
    return info;
}

/**
 * 光电板
 */
DGripper::InfoPkg DGripper::GetInfo_to_Photoelectric() const {
    InfoPkg info;
    info.params = fix.Photoelectric;
    return info;
}

/**
 * 抛料位纯被动，（不是电路板）不用互斥
 */
DGripper::InfoPkg DGripper::GetInfo_to_Throw() const {
    InfoPkg info;
    info.params = fix.Throw;

    emit Event::instance().signal_consume(DeviceConsumeType::WasteTank, 1);
    return info;
}

/**
 * 磁分离板
 */
DGripper::InfoPkg DGripper::GetInfo_to_MagneticSeparation() const {
    InfoPkg info;
    info.params = fix.MagneticSeparation;
    return info;
}

/**
 * 温裕盘
 * TODO
 * 目前还没有锁，后期可能会加上
 */
DGripper::InfoPkg DGripper::GetInfo_to_Load() const {
    InfoPkg info;
    info.params = fix.Load;
    return info;
}

DGripper::InfoPkg DGripper::GetInfo_to_Dilute() const {
    InfoPkg info;
    info.params = fix.Dilute;
    return info;
}

/**
 * 温裕盘
 * TODO
 * 目前还没有锁，后期可能会加上
 */
DGripper::InfoPkg DGripper::GetInfo_to_Incubate_byXY(int i, int j) const {
    InfoPkg info;
    info.params = fix.Load;
    if (fix.Incubate.count(i) && fix.Incubate[i].count(j)) {
        info.params.xPosition = fix.Incubate[i][j].xPosition;
        info.params.yPosition = fix.Incubate[i][j].yPosition;
    }

    return info;
}

void DGripper::Set_ZPositionHeightFlag(bool toTop) {
    auto dataUnit = create_modbusData(1, 1001);
    dataUnit      = read(dataUnit).data;
    dataUnit.setValue(0, set_integerBit(dataUnit.value(0), 8, toTop));
    if (!write(dataUnit).is_valid()) {
        qWarning() << "modbus write error" << ::COM::to_string(dataUnit);
    }
}

}  // namespace COM

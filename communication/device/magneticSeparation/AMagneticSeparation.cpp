#include "AMagneticSeparation.h"

#include "../../config/CKeyJson.hpp"
#include "../../controller/Event.h"
#include "../../utility/UnitConv.h"
#include "../DeviceManager.h"

namespace COM {
AMagneticSeparation::AMagneticSeparation(QObject* parent) : IDeviceThread(parent) {
    DataUnitConfig wconfig;
    wconfig.dataStartAddress = 1000;
    wconfig.deviceAddress    = DeviceAddress_MagneticSeparation;
    set_dataUnitConfig(wconfig);

    QVector<AtomicTask> setFun;
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_None, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_AbleTrayMove, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_AbleWash, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_AbleDrainNeedle, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_AbleASubstratePump, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_HomingTray, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_HomingWashPump, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_HomingASubstratePump, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_HomingZ, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_DebugZ, this));
    setFun.push_back(std::bind(&AMagneticSeparation::execSet_RecoveryError, this));

    QVector<AtomicTask> checkFun;
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_None, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_AbleTrayMove, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_AbleWash, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_AbleDrainNeedle, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_AbleASubstratePump, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_HomingTray, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_HomingWashPump, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_HomingASubstratePump, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_HomingZ, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_DebugZ, this));
    checkFun.push_back(std::bind(&AMagneticSeparation::execCheck_RecoveryError, this));

    assert(setFun.size() == checkFun.size());
    assert(setFun.size() == Mode_Count);
    set_loopFun(setFun, checkFun);
}

QString AMagneticSeparation::to_string() const {
    QString msg = ">>>" + objectName() + "信息: \n";

    msg += Get_mcuVersion() + "\n";
    msg += "Bits: " + Get_mcuStateBits().to_string();
    msg += "Params: " + Get_mcuParams().to_string();

    QModbusDataUnit dataUnit = create_modbusData(2, 1020);
    auto            reply    = read(dataUnit);
    if (reply.is_valid()) {
        dataUnit  = reply.data;
        msg      += QString::asprintf("1020[%d]", dataUnit.value(0));
        msg      += QString::asprintf("1021[%d]", dataUnit.value(1));
        msg      += "\n";
    } else {
        msg += "1020,1021 read-error\n";
    }

    msg += ">>>" + objectName();
    return msg;
}

QString AMagneticSeparation::Get_mcuVersion() const {
    QModbusDataUnit dataUnit = create_modbusData(4, 0);
    auto            reply    = read(dataUnit);
    if (reply.is_valid() == false) {
        return RetCode(RetCode::ReadError).to_string();
    }
    dataUnit = reply.data;

    QString msg = QString("Type[%1] Version[%2.%3.%4]")
                      .arg(dataUnit.value(0))  //
                      .arg(dataUnit.value(1))  //
                      .arg(dataUnit.value(2))  //
                      .arg(dataUnit.value(3));
    return msg;
}

AMagneticSeparation::StateBits AMagneticSeparation::Get_mcuStateBits() const {
    QModbusDataUnit dataUnit = create_modbusData(2, 1002);
    auto            reply    = read(dataUnit);

    dataUnit = reply.data;
    StateBits stateBits;
    stateBits.isValid   = reply.is_valid();
    stateBits.u1002.ALL = dataUnit.value(0);
    stateBits.u1003.ALL = dataUnit.value(1);
    return stateBits;
}

AMagneticSeparation::Params AMagneticSeparation::Get_mcuParams() const {
    QModbusDataUnit dataUnit = create_modbusData(20, 1000);
    auto            reply    = read(dataUnit);

    dataUnit = reply.data;

    ModbusArrayProxyView data(dataUnit, 1000);
    Params               params{};
    params.isValid     = reply.is_valid();
    params.position    = data[1005] << 16u | data[1004];
    params.drainDeep   = data[1013] << 16u | data[1012];
    params.mixHeight   = data[1015] << 16u | data[1014];
    params.zAxisHeight = data[1019] << 16u | data[1018];
    return params;
}

AMagneticSeparation::ErrorPkg AMagneticSeparation::Check_mcuError() const {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return {ErrorCode::SCModbusError, "no valid"};
    }
    return state.get_errorString();
}

/**
 * @brief WMagneticSeparation::set_modeType
 * @param mode
 * @return
 * 设置模式，并检测mode-ready
 */
RetCode AMagneticSeparation::set_modeType(AMagneticSeparation::Mode mode) {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, mode);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    exec_check_modeReady();
    return RetCode::Success;
}

/**
 * @brief WMagneticSeparation::exec_check_modeReady
 */
void AMagneticSeparation::exec_check_modeReady() {
    while (Get_mcuStateBits().u1002.b08_modeReady == 0) {
        QThread::msleep(PeriodMSInterval_Task);
    }
}

RetCode AMagneticSeparation::exec_setUpExec() {
    QModbusDataUnit dataUnit = create_modbusData(1, 1001);
    auto            reply    = read(dataUnit);
    if (reply.is_valid() == false) {
        return RetCode::ReadError;
    }
    dataUnit              = reply.data;
    const uint16_t oldVal = dataUnit.value(0);

    dataUnit.setValue(0, oldVal & (0b1111'1111'1111'1110));
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    dataUnit.setValue(0, oldVal | (0b0000'0000'0000'0001));
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    return RetCode::Success;
}

RetCode AMagneticSeparation::execSet_None() {
    auto ret = set_modeType(Mode_None);
    if (ret != RetCode::Success) {
        return ret;
    }
    return RetCode::Success;
}

RetCode AMagneticSeparation::execCheck_None() {
    return RetCode::Success;
}

/**
 * @brief WMagneticSeparation::execSet_TrayMove
 * @return
 * 常驻模式，不用上升沿
 */
RetCode AMagneticSeparation::execSet_AbleTrayMove() {
    auto ret = set_modeType(Mode_AbleTrayMove);
    if (ret != RetCode::Success) {
        return ret;
    }

    const auto&     sendParam = m_params;
    QModbusDataUnit dataUnit  = create_modbusData(2, 1006);
    dataUnit.setValue(0, sendParam.position);
    dataUnit.setValue(1, sendParam.position >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return RetCode::Success;
}

RetCode AMagneticSeparation::execCheck_AbleTrayMove() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b00_run == 0) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_AbleWash() {
    auto ret = set_modeType(Mode_AbleWash);
    if (ret != RetCode::Success) {
        return ret;
    }

    const auto&     sendParam = m_params;
    QModbusDataUnit dataUnit  = create_modbusData(2, 1008);
    dataUnit.setValue(0, sendParam.washCapacity);
    dataUnit.setValue(1, sendParam.washCapacity >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode AMagneticSeparation::execCheck_AbleWash() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 1) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_AbleDrainNeedle() {
    auto ret = set_modeType(Mode_AbleDrainNeedle);
    if (ret != RetCode::Success) {
        return ret;
    }
    return exec_setUpExec();
}

RetCode AMagneticSeparation::execCheck_AbleDrainNeedle() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 1) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_AbleASubstratePump() {
    auto ret = set_modeType(Mode_AbleASubstratePump);
    if (ret != RetCode::Success) {
        return ret;
    }

    const auto&     sendParam = m_params;
    QModbusDataUnit dataUnit  = create_modbusData(2, 1010);
    dataUnit.setValue(0, sendParam.aSubstrateCapacity);
    dataUnit.setValue(1, sendParam.aSubstrateCapacity >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    int  val = motorStep_to_microlitre_ASubstrate(sendParam.aSubstrateCapacity);
    emit Event::instance().signal_consume(DeviceConsumeType::ASubstrate, val);

    return exec_setUpExec();
}

RetCode AMagneticSeparation::execCheck_AbleASubstratePump() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 1) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_HomingTray() {
    auto ret = set_modeType(Mode_HomingTray);
    if (ret != RetCode::Success) {
        return ret;
    }
    return exec_setUpExec();
}

RetCode AMagneticSeparation::execCheck_HomingTray() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 1) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_HomingWashPump() {
    auto ret = set_modeType(Mode_HomingWashPump);
    if (ret != RetCode::Success) {
        return ret;
    }
    return exec_setUpExec();
}

RetCode AMagneticSeparation::execCheck_HomingWashPump() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 1) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_HomingASubstratePump() {
    auto ret = set_modeType(Mode_HomingASubstratePump);
    if (ret != RetCode::Success) {
        return ret;
    }
    return exec_setUpExec();
}

RetCode AMagneticSeparation::execCheck_HomingASubstratePump() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 1) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_HomingZ() {
    auto ret = set_modeType(Mode_HomingZ);
    if (ret != RetCode::Success) {
        return ret;
    }
    return exec_setUpExec();
}

RetCode AMagneticSeparation::execCheck_HomingZ() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 1) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_DebugZ() {
    auto ret = set_modeType(Mode_DebugZ);
    if (ret != RetCode::Success) {
        return ret;
    }

    const auto&     sendParam = m_params;
    QModbusDataUnit dataUnit  = create_modbusData(2, 1016);
    dataUnit.setValue(0, sendParam.zAxisHeight);
    dataUnit.setValue(1, sendParam.zAxisHeight >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return exec_setUpExec();
}

RetCode AMagneticSeparation::execCheck_DebugZ() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b01_zAxisRun == 0) {
        if (state.u1002.b04_refuse == 1) {
            return RetCode::RefuseError;
        }
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AMagneticSeparation::execSet_RecoveryError() {
    QModbusDataUnit dataUnit = create_modbusData(1, 1001);

    dataUnit.setValue(0, 0);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    dataUnit.setValue(0, 0x8000);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    return RetCode::Success;
}

RetCode AMagneticSeparation::execCheck_RecoveryError() {
    return RetCode::Success;
}

}  // namespace COM

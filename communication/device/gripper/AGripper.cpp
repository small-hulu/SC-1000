#include "AGripper.h"

#include <QDebug>
#include <QEventLoop>
#include <QModbusClient>

#include "../../config/CKeyJson.hpp"
#include "../../utility/PDT.h"
#include "../DeviceManager.h"

namespace COM {
AGripper::AGripper(QObject *parent) : IDeviceThread(parent) {
    DataUnitConfig wconfig;
    wconfig.dataStartAddress = 1000;
    wconfig.deviceAddress    = DeviceAddress_Gripper;
    set_dataUnitConfig(wconfig);

    QVector<AtomicTask> setFun;
    setFun.push_back(std::bind(&AGripper::execSet_None, this));
    setFun.push_back(std::bind(&AGripper::execSet_XYPosition, this));
    setFun.push_back(std::bind(&AGripper::execSet_Grab, this));
    setFun.push_back(std::bind(&AGripper::execSet_Release, this));
    setFun.push_back(std::bind(&AGripper::execSet_HardBrake, this));
    setFun.push_back(std::bind(&AGripper::execSet_Homing, this));
    setFun.push_back(std::bind(&AGripper::execSet_XYZPosition, this));
    setFun.push_back(std::bind(&AGripper::execSet_MoveGrab, this));
    setFun.push_back(std::bind(&AGripper::execSet_MoveRelease, this));
    setFun.push_back(std::bind(&AGripper::execSet_RecoveryError, this));

    QVector<AtomicTask> checkFun;
    checkFun.push_back(std::bind(&AGripper::execCheck_None, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_XYPosition, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_Grab, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_Release, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_HardBrake, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_Homing, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_XYZPosition, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_MoveGrab, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_MoveRelease, this));
    checkFun.push_back(std::bind(&AGripper::execCheck_RecoveryError, this));

    assert(setFun.size() == checkFun.size());
    assert(setFun.size() == Mode_Count);
    set_loopFun(setFun, checkFun);
}

QString AGripper::to_string() const {
    QString msg = ">>>" + objectName() + "信息: \n";

    msg += Get_mcuVersion() + "\n";
    msg += "Bits: " + Get_mcuStateBits().to_string();
    msg += "Params: " + Get_mcuParams().to_string();

    QModbusDataUnit dataUnit = create_modbusData(2, 1022);
    auto            reply    = read(dataUnit);
    if (reply.is_valid()) {
        dataUnit  = reply.data;
        msg      += QString::asprintf("1022[%d]", dataUnit.value(0));
        msg      += QString::asprintf("1023[%d]", dataUnit.value(1));
        msg      += "\n";
    } else {
        msg += "1022,1023 read-error\n";
    }

    msg += ">>>" + objectName();
    return msg;
}

QString AGripper::Get_mcuVersion() const {
    QModbusDataUnit dataUnit = create_modbusData(4, 0);
    auto            reply    = read(dataUnit);
    if (reply.is_valid() == false) {
        return RetCode(RetCode::ReadError).to_string();
    }
    dataUnit    = reply.data;
    QString msg = QString("Type[%1] Version[%2.%3.%4]")
                      .arg(dataUnit.value(0))  //
                      .arg(dataUnit.value(1))  //
                      .arg(dataUnit.value(2))  //
                      .arg(dataUnit.value(3));
    return msg;
}

AGripper::StateBits AGripper::Get_mcuStateBits() const {
    QModbusDataUnit dataUnit = create_modbusData(2, 1002);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    StateBits stateBits;
    stateBits.isValid   = reply.is_valid();
    stateBits.u1002.ALL = dataUnit.value(0);
    stateBits.u1003.ALL = dataUnit.value(1);
    return stateBits;
}

AGripper::Params AGripper::Get_mcuParams() const {
    QModbusDataUnit dataUnit = create_modbusData(28);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    ModbusArrayProxyView data(dataUnit, get_dataUnitConfig().dataStartAddress);
    Params               params{};
    params.isValid   = reply.is_valid();
    params.xPosition = twou16_to_oneu32<uint32_t>(data[1004], data[1005]);
    params.yPosition = twou16_to_oneu32<uint32_t>(data[1006], data[1007]);
    params.zPosition = twou16_to_oneu32<uint32_t>(data[1008], data[1009]);
    params.openRange = twou16_to_oneu32<uint32_t>(data[1020], data[1021]);
    params.rangeMin  = twou16_to_oneu32<uint32_t>(data[1024], data[1025]);
    params.rangeMax  = twou16_to_oneu32<uint32_t>(data[1026], data[1027]);
    return params;
}

AGripper::ErrorPkg AGripper::Check_mcuError() const {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return {ErrorCode::SCModbusError, "no valid"};
    }
    return state.get_errorString();
}

RetCode AGripper::solve_mcuError() {
    auto state = Get_mcuStateBits();
    if (state.u1003.b01_ErrorAssembly) {
        Set_Config(Mode_Homing, m_params);
        Exec_sequencedTask();
        return RetCode::MCUError;
    }
    return RetCode::Success;
}

/**
 * @brief WGripper::set_modeType
 * @param mode
 * @return
 * 仅做模式标志的设置
 */
RetCode AGripper::set_modeType(AGripper::Mode mode) {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, mode);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    exec_check_modeReady();
    return RetCode::Success;
}

/**
 * @brief WGripper::exec_check_modeReady
 * 检验模式准备完成
 */
void AGripper::exec_check_modeReady() {
    while (Get_mcuStateBits().u1002.b08_modeReady == 0) {
        QThread::msleep(PeriodMSInterval_Task);
    }
}

/**
 * @brief TGripper::exec_setUpExec
 * 需要触发动作执行寄存器
 * 写0再写1
 */
RetCode AGripper::exec_setUpExec() {
    QModbusDataUnit dataUnit = create_modbusData(1, 1001);

    dataUnit.setValue(0, 0);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    dataUnit.setValue(0, 1);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    return RetCode::Success;
}

RetCode AGripper::execSet_None() {
    auto ret = set_modeType(Mode_None);
    if (ret != RetCode::Success) {
        return ret;
    }

    return exec_setUpExec();
}

RetCode AGripper::execCheck_None() {
    return RetCode::Success;
}

/**
 * @brief WGripper::execSet_XYPosition
 * @return
 * 常驻模式，不用上升延
 */
RetCode AGripper::execSet_XYPosition() {
    auto ret = set_modeType(Mode_XYPosition);
    if (ret != RetCode::Success) {
        return ret;
    }

    CGripper::Params sendParams = m_params;
    QModbusDataUnit  dataUnit   = create_modbusData(4, 1010);
    uint16_t         low, high;

    std::tie(low, high) = oneu32_to_twou16(sendParams.xPosition);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    std::tie(low, high) = oneu32_to_twou16(sendParams.yPosition);
    dataUnit.setValue(2, low);
    dataUnit.setValue(3, high);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return RetCode::Success;
}

/**
 * @brief AGripper::execCheck_XYPosition
 * @return
 * 常驻模式，一般不会拒接执行
 */
RetCode AGripper::execCheck_XYPosition() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b02_xyAxisReach == 1) {
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AGripper::execSet_Grab() {
    auto ret = set_modeType(Mode_Grab);
    if (ret != RetCode::Success) {
        return ret;
    }

    CGripper::Params sendParams = m_params;
    QModbusDataUnit  dataUnit   = create_modbusData(2, 1016);
    uint16_t         low, high;
    std::tie(low, high) = oneu32_to_twou16<int32_t>(sendParams.deepGrab);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode AGripper::execCheck_Grab() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b05_success == 1) {
        if (state.u1002.b07_itemExist == 1) {
            return RetCode::Success;
        } else {
            return RetCode::ItemExistError;
        }
    }
    if (state.u1002.b03_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qWarning() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode AGripper::execSet_Release() {
    auto ret = set_modeType(Mode_Release);
    if (ret != RetCode::Success) {
        return ret;
    }

    CGripper::Params sendParams = m_params;
    QModbusDataUnit  dataUnit   = create_modbusData(2, 1018);
    uint16_t         low, high;
    std::tie(low, high) = oneu32_to_twou16<int32_t>(sendParams.deepRelease);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode AGripper::execCheck_Release() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b05_success == 1) {
        if (state.u1002.b07_itemExist == 0) {
            return RetCode::Success;
        } else {
            return RetCode::ItemExistError;
        }
    }
    if (state.u1002.b03_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qWarning() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode AGripper::execSet_HardBrake() {
    auto ret = set_modeType(Mode_HardBrake);
    if (ret != RetCode::Success) {
        return ret;
    }

    return exec_setUpExec();
}

RetCode AGripper::execCheck_HardBrake() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b05_success == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b03_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qWarning() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode AGripper::execSet_Homing() {
    auto ret = set_modeType(Mode_Homing);
    if (ret != RetCode::Success) {
        return ret;
    }

    return exec_setUpExec();
}

RetCode AGripper::execCheck_Homing() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b05_success == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b03_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qWarning() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode AGripper::execSet_XYZPosition() {
    auto ret = set_modeType(Mode_XYZPosition);
    if (ret != RetCode::Success) {
        return ret;
    }

    CGripper::Params sendParams = m_params;
    QModbusDataUnit  dataUnit   = create_modbusData(12, 1010);
    uint16_t         low, high;

    std::tie(low, high) = oneu32_to_twou16(sendParams.xPosition);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    std::tie(low, high) = oneu32_to_twou16(sendParams.yPosition);
    dataUnit.setValue(2, low);
    dataUnit.setValue(3, high);
    std::tie(low, high) = oneu32_to_twou16(sendParams.zPosition);
    dataUnit.setValue(4, low);
    dataUnit.setValue(5, high);
    std::tie(low, high) = oneu32_to_twou16(sendParams.openRange);
    dataUnit.setValue(10, low);
    dataUnit.setValue(11, high);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return RetCode::Success;
}

/**
 * 调试模式 无脑成功
 */
RetCode AGripper::execCheck_XYZPosition() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b02_xyAxisReach == 1) {
        return RetCode::Success;
    }
    return RetCode::Running;
}

RetCode AGripper::execSet_MoveGrab() {
    auto ret = set_modeType(Mode_MoveGrab);
    if (ret != RetCode::Success) {
        return ret;
    }

    CGripper::Params sendParams = m_params;
    QModbusDataUnit  dataUnit   = create_modbusData(8, 1010);
    uint16_t         low, high;

    std::tie(low, high) = oneu32_to_twou16(sendParams.xPosition);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    std::tie(low, high) = oneu32_to_twou16(sendParams.yPosition);
    dataUnit.setValue(2, low);
    dataUnit.setValue(3, high);
    std::tie(low, high) = oneu32_to_twou16(sendParams.deepGrab);
    dataUnit.setValue(6, low);
    dataUnit.setValue(7, high);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode AGripper::execCheck_MoveGrab() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b05_success == 1) {
        if (state.u1002.b07_itemExist == 1) {
            return RetCode::Success;
        } else {
            return RetCode::ItemExistError;
        }
    }
    if (state.u1002.b03_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qWarning() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode AGripper::execSet_MoveRelease() {
    auto ret = set_modeType(Mode_MoveRelease);
    if (ret != RetCode::Success) {
        return ret;
    }

    CGripper::Params sendParams = m_params;
    QModbusDataUnit  dataUnit   = create_modbusData(10, 1010);
    uint16_t         low, high;

    std::tie(low, high) = oneu32_to_twou16(sendParams.xPosition);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    std::tie(low, high) = oneu32_to_twou16(sendParams.yPosition);
    dataUnit.setValue(2, low);
    dataUnit.setValue(3, high);
    std::tie(low, high) = oneu32_to_twou16(sendParams.deepRelease);
    dataUnit.setValue(8, low);
    dataUnit.setValue(9, high);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode AGripper::execCheck_MoveRelease() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b12_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b05_success == 1) {
        if (state.u1002.b07_itemExist == 0) {
            return RetCode::Success;
        } else {
            return RetCode::ItemExistError;
        }
    }
    if (state.u1002.b03_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qWarning() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

/**
 * 错误恢复，在1001上的bit[15]做上升沿
 */
RetCode AGripper::execSet_RecoveryError() {
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

RetCode AGripper::execCheck_RecoveryError() {
    return RetCode::Success;
}

}  // namespace COM

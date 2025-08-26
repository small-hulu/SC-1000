/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       APipetteNeedle.cpp
 * @date       2024-07-13
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 先判断b10
 * 再b03
 * 最后是其他具体错误
 *
 *****************************************************************************/
#include "APipetteNeedle.h"

#include "../../config/CKeyJson.hpp"
#include "../../utility/PDT.h"
#include "../DeviceManager.h"

namespace COM {

APipetteNeedle::APipetteNeedle(QObject* parent) : IDeviceThread(parent) {
    DataUnitConfig wconfig;
    wconfig.dataStartAddress = 1000;
    wconfig.deviceAddress    = DeviceAddress_PipettingNeedle;
    set_dataUnitConfig(wconfig);

    QVector<AtomicTask> setFun;
    setFun.push_back(std::bind(&APipetteNeedle::execSet_None, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_YPosition, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_Suction, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_Drainage, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_HardBrake, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_Wash, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_Homing, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_FillBottle, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_WashOutside, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_MoveSuction, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_MoveDrainage, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_DebugYZPosition, this));
    setFun.push_back(std::bind(&APipetteNeedle::execSet_RecoveryError, this));

    QVector<AtomicTask> checkFun;
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_None, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_YPosition, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_Suction, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_Drainage, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_HardBrake, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_Wash, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_Homing, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_FillBottle, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_WashOutside, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_MoveSuction, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_MoveDrainage, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_DebugYZPosition, this));
    checkFun.push_back(std::bind(&APipetteNeedle::execCheck_RecoveryError, this));

    assert(setFun.size() == checkFun.size());
    assert(setFun.size() == Mode_Count);
    set_loopFun(setFun, checkFun);
}

QString APipetteNeedle::to_string() const {
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

QString APipetteNeedle::Get_mcuVersion() const {
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

APipetteNeedle::StateBits APipetteNeedle::Get_mcuStateBits() const {
    QModbusDataUnit dataUnit = create_modbusData(2, 1002);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    StateBits stateBits;
    stateBits.isValid   = reply.is_valid();
    stateBits.u1002.ALL = dataUnit.value(0);
    stateBits.u1003.ALL = dataUnit.value(1);
    return stateBits;
}

APipetteNeedle::Params APipetteNeedle::Get_mcuParams() const {
    QModbusDataUnit dataUnit = create_modbusData(31);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    ModbusArrayProxyView data(dataUnit, get_dataUnitConfig().dataStartAddress);
    Params               params{};
    params.isValid           = reply.is_valid();
    params.yPosition         = twou16_to_oneu32<int32_t>(data[1004], data[1005]);
    params.zPosition         = twou16_to_oneu32<int32_t>(data[1006], data[1007]);
    params.suctionZPosition  = twou16_to_oneu32<int32_t>(data[1010], data[1011]);
    params.drainageZPosition = twou16_to_oneu32<int32_t>(data[1012], data[1013]);
    params.suctionCapacity   = twou16_to_oneu32<int32_t>(data[1014], data[1015]);
    params.washYPosition     = twou16_to_oneu32<int32_t>(data[1016], data[1017]);
    params.washZPosition     = twou16_to_oneu32<int32_t>(data[1018], data[1019]);
    params.descentCache      = twou16_to_oneu32<int32_t>(data[1026], data[1027]);
    params.delayWashTime     = twou16_to_oneu32<int32_t>(data[1028], data[1029]);

    return params;
}

APipetteNeedle::ErrorPkg APipetteNeedle::Check_mcuError() const {
    auto state = Get_mcuStateBits();
    /// 通信异常
    if (state.isValid == false) {
        return {ErrorCode::SCModbusError, "no valid"};
    }
    return state.get_errorString();
}

/**
 * 修改模式时，检测模式是否完成
 */
void APipetteNeedle::exec_check_modeReady() {
    while (Get_mcuStateBits().u1002.b08_modeReady == 0) {
        QThread::msleep(PeriodMSInterval_Task);
    }
}

/**
 * 需要触发动作执行寄存器
 * 当检测到1时才触发
 * 因此先写0再写1
 * ======================================
 * 在写入后检测是都拒绝执行
 */
RetCode APipetteNeedle::exec_setUpExec() {
    exec_check_modeReady();
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

RetCode APipetteNeedle::execSet_None() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_None);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return RetCode::Success;
}

RetCode APipetteNeedle::execCheck_None() {
    return RetCode::Success;
}

/**
 * y轴运动是*常驻模式*
 * 且只有y是常驻模式
 * 1. 设置模式
 * 2. 等待模式准备完成
 * 3. 设置位置
 */
RetCode APipetteNeedle::execSet_YPosition() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_YPosition);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    exec_check_modeReady();

    Params sendParams = m_params;
    dataUnit          = create_modbusData(2, 1008);
    dataUnit.setValue(0, sendParams.yPosition);
    dataUnit.setValue(1, sendParams.yPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return RetCode::Success;
}

/**
 * y轴运动属于特殊模式
 * 只判断一个状态位即可
 */
RetCode APipetteNeedle::execCheck_YPosition() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b02_yAxisReached == 0) {
        return RetCode::Running;
    }

    return RetCode::Success;
}

/**
 * 吸液模式
 */
RetCode APipetteNeedle::execSet_Suction() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_Suction);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    Params sendParams = m_params;
    /// 设置深度
    dataUnit          = create_modbusData(2, 1010);
    dataUnit.setValue(0, sendParams.suctionZPosition);
    dataUnit.setValue(1, sendParams.suctionZPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    /// 设置吸液量
    dataUnit.setStartAddress(1014);
    dataUnit.setValue(0, sendParams.suctionCapacity);
    dataUnit.setValue(1, sendParams.suctionCapacity >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    /// 设置吸液门限
    dataUnit = create_modbusData(1, 1020);
    dataUnit.setValue(0, sendParams.suctionThreshold);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

/**
 * 吸液成功
 * 试剂不足
 */
RetCode APipetteNeedle::execCheck_Suction() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b05_InsufficientReagent_in == 1) {
        return RetCode::Insufficient_in;
    }
    if (state.u1002.b11_InsufficientReagent_out == 1) {
        return RetCode::Insufficient_out;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode APipetteNeedle::execSet_Drainage() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_Drainage);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    Params sendParams = m_params;
    dataUnit          = create_modbusData(2, 1012);
    dataUnit.setValue(0, sendParams.drainageZPosition);
    dataUnit.setValue(1, sendParams.drainageZPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

/**
 * 成功和拒接执行
 */
RetCode APipetteNeedle::execCheck_Drainage() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode APipetteNeedle::execSet_HardBrake() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_HardBrake);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode APipetteNeedle::execCheck_HardBrake() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

/**
 * 洗针需要设置洗针位
 * 会先自动过去再自动洗针
 */
RetCode APipetteNeedle::execSet_Wash() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_Wash);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    Params sendParams = m_params;
    dataUnit          = create_modbusData(4, 1016);        //取四个寄存器 1016~1019
    dataUnit.setValue(0, sendParams.washYPosition);        //操作第一个
    dataUnit.setValue(1, sendParams.washYPosition >> 16u); //操作第二个
    dataUnit.setValue(2, sendParams.washZPosition);
    dataUnit.setValue(3, sendParams.washZPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

/**
 * 撞了的话，就是拒绝执行
 */
RetCode APipetteNeedle::execCheck_Wash() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode APipetteNeedle::execSet_Homing() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_Homing);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode APipetteNeedle::execCheck_Homing() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode APipetteNeedle::execSet_FillBottle() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_FillBottle);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

/**
 * 目前没有检测补充瓶能正常完成的标志和方案
 */
RetCode APipetteNeedle::execCheck_FillBottle() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode APipetteNeedle::execSet_WashOutside() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_WashOutside);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    Params sendParams = m_params;
    dataUnit          = create_modbusData(4, 1016);
    dataUnit.setValue(0, sendParams.washYPosition);
    dataUnit.setValue(1, sendParams.washYPosition >> 16u);
    dataUnit.setValue(2, sendParams.washZPosition);
    dataUnit.setValue(3, sendParams.washZPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode APipetteNeedle::execCheck_WashOutside() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode APipetteNeedle::execSet_MoveSuction() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_MoveSuction);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    Params sendParams = m_params;
    /// 设置y轴移动
    dataUnit          = create_modbusData(2, 1008);
    dataUnit.setValue(0, sendParams.yPosition);
    dataUnit.setValue(1, sendParams.yPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    /// 设置深度
    dataUnit = create_modbusData(2, 1010);
    dataUnit.setValue(0, sendParams.suctionZPosition);
    dataUnit.setValue(1, sendParams.suctionZPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    /// 设置吸液量
    dataUnit.setStartAddress(1014);
    dataUnit.setValue(0, sendParams.suctionCapacity);
    dataUnit.setValue(1, sendParams.suctionCapacity >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    /// 设置吸液门限
    dataUnit = create_modbusData(1, 1020);
    dataUnit.setValue(0, sendParams.suctionThreshold);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode APipetteNeedle::execCheck_MoveSuction() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b05_InsufficientReagent_in == 1) {
        return RetCode::Insufficient_in;
    }
    if (state.u1002.b11_InsufficientReagent_out == 1) {
        return RetCode::Insufficient_out;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode APipetteNeedle::execSet_MoveDrainage() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_MoveDrainage);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    Params sendParams = m_params;
    /// 设置y轴移动
    dataUnit          = create_modbusData(2, 1008);
    dataUnit.setValue(0, sendParams.yPosition);
    dataUnit.setValue(1, sendParams.yPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    dataUnit = create_modbusData(2, 1012);
    dataUnit.setValue(0, sendParams.drainageZPosition);
    dataUnit.setValue(1, sendParams.drainageZPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_setUpExec();
}

RetCode APipetteNeedle::execCheck_MoveDrainage() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b10_isLeisure == 0) {
        return RetCode::Running;
    }
    if (state.u1002.b03_actionSuccess == 1) {
        return RetCode::Success;
    }
    if (state.u1002.b12_refuse == 1) {
        return RetCode::RefuseError;
    }
    if (state.u1002.b04_crash == 1) {
        return RetCode::CrashError;
    }

    qCritical() << "u1002's val =" << state.u1002.ALL;
    return RetCode::UnknownError;
}

RetCode APipetteNeedle::execSet_DebugYZPosition() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_DebugYZPosition);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    exec_check_modeReady();

    Params sendParams = m_params;
    /// 设置y轴
    dataUnit          = create_modbusData(2, 1008);
    dataUnit.setValue(0, sendParams.yPosition);
    dataUnit.setValue(1, sendParams.yPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    /// 设置z轴
    dataUnit = create_modbusData(2, 1024);
    dataUnit.setValue(0, sendParams.zPosition);
    dataUnit.setValue(1, sendParams.zPosition >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return RetCode::Success;
}

RetCode APipetteNeedle::execCheck_DebugYZPosition() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b02_yAxisReached == 0) {
        return RetCode::Running;
    }

    return RetCode::Success;
}

RetCode APipetteNeedle::execSet_RecoveryError() {
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

RetCode APipetteNeedle::execCheck_RecoveryError() {
    return RetCode::Success;
}

}  // namespace COM

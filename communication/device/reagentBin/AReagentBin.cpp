/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       WorkerThread_ReagentPlate.cpp
 * @date       2024-01-03
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      File Description
 * 注意一下，这里的两个操作
 * - 设置位置
 * - 设置速度
 * 这个两个操作的是十分类似的，从两个函数的内部格式形式上可以看出来
 * 但是为什么不单独拎出来一个函数，并传入参数操作呢
 * 其实现在逻辑比较简单是可以的
 * 但是，从业务逻辑层面上来看，这就是两个不同的操作，就是应该分别处理
 * 分别应对后期可能出现的变化
 * =======================================================
 * 6400 * 3 一圈(脉冲数)
 *****************************************************************************/
#include "AReagentBin.h"

#include <QDebug>
#include <QModbusClient>

#include "../../config/CKeyJson.hpp"
#include "../../utility/ToString.h"
#include "../DeviceManager.h"

namespace COM {
/**
 * @brief ReagentPlate::ReagentPlate
 * @param parent
 * QThread Object
 */
AReagentBin::AReagentBin(QObject* parent) : IDeviceThread(parent) {
    DataUnitConfig wconfig;
    wconfig.dataStartAddress = 1000;
    wconfig.deviceAddress    = DeviceAddress_ReagentBin;
    set_dataUnitConfig(wconfig);

    QVector<AtomicTask> setFun;
    setFun.push_back(std::bind(&AReagentBin::execSet_None, this));
    setFun.push_back(std::bind(&AReagentBin::execSet_Position, this));
    setFun.push_back(std::bind(&AReagentBin::execSet_Velocity, this));
    setFun.push_back(std::bind(&AReagentBin::execSet_BrakeSoft, this));
    setFun.push_back(std::bind(&AReagentBin::execSet_BrakeHard, this));
    setFun.push_back(std::bind(&AReagentBin::execSet_Homing, this));
    setFun.push_back(std::bind(&AReagentBin::execSet_LED_RGB, this));
    setFun.push_back(std::bind(&AReagentBin::execSet_RecoveryError, this));

    QVector<AtomicTask> checkFun;
    checkFun.push_back(std::bind(&AReagentBin::execCheck_None, this));
    checkFun.push_back(std::bind(&AReagentBin::execCheck_Position, this));
    checkFun.push_back(std::bind(&AReagentBin::execCheck_Velocity, this));
    checkFun.push_back(std::bind(&AReagentBin::execCheck_BrakeSoft, this));
    checkFun.push_back(std::bind(&AReagentBin::execCheck_BrakeHard, this));
    checkFun.push_back(std::bind(&AReagentBin::execCheck_Homing, this));
    checkFun.push_back(std::bind(&AReagentBin::execCheck_LED_RGB, this));
    checkFun.push_back(std::bind(&AReagentBin::execCheck_RecoveryError, this));

    assert(setFun.size() == checkFun.size());
    assert(setFun.size() == Mode_Count);
    set_loopFun(setFun, checkFun);
}

/**
 * 打印当前状态的信息
 */
QString AReagentBin::to_string() const {
    QString msg = ">>>" + objectName() + "信息: \n";

    msg += Get_mcuVersion() + "\n";
    msg += "Bits: " + Get_mcuStateBits().to_string();
    msg += "Params: " + Get_mcuParams().to_string();

    QModbusDataUnit dataUnit = create_modbusData(2, 1012);
    auto            reply    = read(dataUnit);
    if (reply.is_valid()) {
        dataUnit  = reply.data;
        msg      += QString::asprintf("1012[%d]", dataUnit.value(0));
        msg      += QString::asprintf("1013[%d]", dataUnit.value(1));
        msg      += "\n";
    } else {
        msg += "1012,1013 read-error\n";
    }
    msg += ">>>" + objectName();
    return msg;
}

QString AReagentBin::Get_mcuVersion() const {
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

/**
 * 使用位于优化空间
 * 否则状态太多，传递开销太大
 */
AReagentBin::StateBits AReagentBin::Get_mcuStateBits() const {
    QModbusDataUnit dataUnit = create_modbusData(2, 1002);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    StateBits stateBits;
    stateBits.isValid   = reply.is_valid();
    stateBits.u1002.ALL = dataUnit.value(0);
    stateBits.u1003.ALL = dataUnit.value(1);
    return stateBits;
}

/**
 * ModbusBase 要求同步读写
 * 请根据具体协议来进行设置
 */
AReagentBin::Params AReagentBin::Get_mcuParams() const {
    QModbusDataUnit dataUnit = create_modbusData(17);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    int    offset = get_dataUnitConfig().dataStartAddress;
    Params params{};
    params.isValid  = reply.is_valid();
    params.position = dataUnit.value(1005 - offset) << 16u | dataUnit.value(1004 - offset);
    params.velocity = dataUnit.value(1007 - offset) << 16u | dataUnit.value(1006 - offset);
    params.LED_R    = dataUnit.value(1014 - offset);
    params.LED_G    = dataUnit.value(1015 - offset);
    params.LED_B    = dataUnit.value(1016 - offset);
    return params;
}

AReagentBin::ErrorPkg AReagentBin::Check_mcuError() const {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return {ErrorCode::SCModbusError, "no valid"};
    }
    return state.get_errorString();
}

/**
 * 上升沿
 */
RetCode AReagentBin::exec_setUpExec() {
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

/**
 * 设置保留模式
 */
RetCode AReagentBin::execSet_None() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_None);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return exec_setUpExec();
}

/**
 * 保留位置，目前直接true
 */
RetCode AReagentBin::execCheck_None() {
    return RetCode::Success;
}

/**
 * 位置模式
 * 注意：位置可能会涉及取模问题
 */
RetCode AReagentBin::execSet_Position() {
    /// 设置目标位置
    Params          sendConfig = m_params;
    QModbusDataUnit dataUnit;

    dataUnit = create_modbusData(2, 1008);
    dataUnit.setValue(0, sendConfig.position);
    dataUnit.setValue(1, sendConfig.position >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    /// 改变模式寄存器到位置模式
    dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_Position);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    dataUnit = create_modbusData(2, 1008);
    dataUnit.setValue(0, sendConfig.position);
    dataUnit.setValue(1, sendConfig.position >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return RetCode::Success;
}

/**
 * 位置是否达标
 */
RetCode AReagentBin::execCheck_Position() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b02_isReachPosition == 1) {
        return RetCode::Success;
    }
    return RetCode::Running;
}

/**
 * 速度模式
 */
RetCode AReagentBin::execSet_Velocity() {
    /// 设置目标速度
    Params          sendConfig = m_params;
    QModbusDataUnit dataUnit   = create_modbusData(2, 1010);
    dataUnit.setValue(0, sendConfig.velocity);
    dataUnit.setValue(1, sendConfig.velocity >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    /// 设定速度模式
    dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_Velocity);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return RetCode::Success;
}

/**
 * 速度是否达标
 */
RetCode AReagentBin::execCheck_Velocity() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1002.b03_isReachVelocity == 1) {
        return RetCode::Success;
    }
    return RetCode::Running;
}

/**
 * 软刹车，会慢慢停
 */
RetCode AReagentBin::execSet_BrakeSoft() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_BrakeSoft);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return exec_setUpExec();
}

/**
 * 刹车，速度是否为0
 */
RetCode AReagentBin::execCheck_BrakeSoft() {
    auto params = Get_mcuParams();
    if (params.isValid == false) {
        return RetCode::ReadError;
    }
    if (params.velocity == 0) {
        return RetCode::Success;
    }
    return RetCode::Running;
}

/**
 * 急刹车，强制停止
 * 急刹车后需要重新回原点
 */
RetCode AReagentBin::execSet_BrakeHard() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_BrakeHard);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return exec_setUpExec();
}

/**
 * 速度是否为0
 */
RetCode AReagentBin::execCheck_BrakeHard() {
    auto params = Get_mcuParams();
    if (params.isValid == false) {
        return RetCode::ReadError;
    }
    if (params.velocity == 0) {
        return RetCode::Success;
    }
    return RetCode::Running;
}

/**
 * 回原点
 */
RetCode AReagentBin::execSet_Homing() {
    QModbusDataUnit dataUnit = create_modbusData(1);
    dataUnit.setValue(0, Mode_Homing);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    return exec_setUpExec();
}

/**
 * 状态位置的home是否置1
 */
RetCode AReagentBin::execCheck_Homing() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1003.b07_homing == 1) {
        return RetCode::Success;
    }
    return RetCode::Running;
}

/**
 * 设置 RGB 颜色，直接写入即可
 */
RetCode AReagentBin::execSet_LED_RGB() {
    QModbusDataUnit dataUnit = create_modbusData(3, 1014);
    dataUnit.setValue(0, m_params.LED_R);
    dataUnit.setValue(1, m_params.LED_G);
    dataUnit.setValue(2, m_params.LED_B);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return RetCode::Success;
}

/**
 * RGB 是立即设置型
 */
RetCode AReagentBin::execCheck_LED_RGB() {
    return RetCode::Success;
}

RetCode AReagentBin::execSet_RecoveryError() {
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

RetCode AReagentBin::execCheck_RecoveryError() {
    return RetCode::Success;
}

}  // namespace COM

#include "APhotoelectric.h"

#include "../../config/CKeyJson.hpp"
#include "../../controller/Event.h"
#include "../../utility/PDT.h"
#include "../../utility/UnitConv.h"
#include "../DeviceManager.h"

namespace COM {
APhotoelectric::APhotoelectric(QObject *parent) : IDeviceThread(parent) {
    DataUnitConfig wconfig;
    wconfig.dataStartAddress = DeviceStartAddress;
    wconfig.deviceAddress    = DeviceAddress_Photoelectric;
    set_dataUnitConfig(wconfig);

    QVector<AtomicTask> setFun;
    setFun.push_back(std::bind(&APhotoelectric::execSet_None, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_DoorClose, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_DoorOpen, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_Detect, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_HomingPump, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_OnlyDetect, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_OnlyInjection, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_HomingDoor, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_DebugDoor, this));
    setFun.push_back(std::bind(&APhotoelectric::execSet_RecoveryError, this));

    QVector<AtomicTask> checkFun;
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_None, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_DoorClose, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_DoorOpen, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_Detect, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_HomingPump, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_OnlyDetect, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_OnlyInjection, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_HomingDoor, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_DebugDoor, this));
    checkFun.push_back(std::bind(&APhotoelectric::execCheck_RecoveryError, this));

    assert(setFun.size() == checkFun.size());
    assert(setFun.size() == Mode_Count);
    set_loopFun(setFun, checkFun);
}

QString APhotoelectric::to_string() const {
    QString msg = ">>>" + objectName() + "信息: \n";

    msg += Get_mcuVersion() + "\n";
    msg += "Bits: " + Get_mcuStateBits().to_string();
    msg += "Params: " + Get_mcuParams().to_string();

    msg += ">>>" + objectName();
    return msg;
}

QString APhotoelectric::Get_mcuVersion() const {
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

APhotoelectric::StateBits APhotoelectric::Get_mcuStateBits() const {
    QModbusDataUnit dataUnit = create_modbusData(1, 1001);
    auto            reply    = read(dataUnit);

    dataUnit = reply.data;

    StateBits stateBits;
    stateBits.isValid   = reply.is_valid();
    stateBits.u1001.ALL = dataUnit.value(0);
    return stateBits;
}

APhotoelectric::Params APhotoelectric::Get_mcuParams() const {
    QModbusDataUnit dataUnit = create_modbusData(10);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    ModbusArrayProxyView data(dataUnit, get_dataUnitConfig().dataStartAddress);
    Params               params{};
    params.isValid    = reply.is_valid();
    params.capacity   = twou16_to_oneu32<int32_t>(data[1004], data[1005]);
    params.positionIn = twou16_to_oneu32<int32_t>(data[1006], data[1007]);
    params.position   = twou16_to_oneu32<int32_t>(data[1008], data[1009]);
    return params;
}

APhotoelectric::ErrorPkg APhotoelectric::Check_mcuError() const {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return {ErrorCode::SCModbusError, "no valid"};
    }
    return state.get_errorString();
}

/**
 * 需要在执行检测后再操作
 */
int APhotoelectric::Get_value() {
    QModbusDataUnit dataUnit = create_modbusData(2, 1002);
    auto            reply    = read(dataUnit);
    if (reply.is_valid() == false) {
        return -1;
    }
    dataUnit = reply.data;

    int32_t val = dataUnit.value(1) << 16u | dataUnit.value(0);
    return val;
}

/**
 * 单个点的上升沿
 */
RetCode APhotoelectric::exec_risingEdge(int startAddress, int bitAddress) {
    QModbusDataUnit dataUnit = create_modbusData(1, startAddress);

    dataUnit.setValue(0, 0);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    dataUnit.setValue(0, (1u << bitAddress));
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    return RetCode::Success;
}

RetCode APhotoelectric::execSet_None() {
    return RetCode::Success;
}

RetCode APhotoelectric::execCheck_None() {
    return RetCode::Success;
}

/**
 * 关闭仓
 * 但是切换到别的模式时，比如检测和泵归位的时候
 * 下位机都会先自动归位
 */
RetCode APhotoelectric::execSet_DoorClose() {
    QModbusDataUnit dataUnit = create_modbusData(1);

    dataUnit.setValue(0, 1);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    dataUnit.setValue(0, 0);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }
    QThread::msleep(PeriodMSInterval_Task);

    return RetCode::Success;
}

RetCode APhotoelectric::execCheck_DoorClose() {
    auto state = Get_mcuStateBits();
    if (state.u1001.b00_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1001.b01_success == 1) {
        return RetCode::Success;
    }
    return RetCode::UnknownError;
}

/**
 * 打开光电仓
 */
RetCode APhotoelectric::execSet_DoorOpen() {
    return exec_risingEdge(1000, 0);
}

RetCode APhotoelectric::execCheck_DoorOpen() {
    auto state = Get_mcuStateBits();
    if (state.u1001.b00_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1001.b01_success == 1) {
        return RetCode::Success;
    }
    return RetCode::UnknownError;
}

/**
 * 需要泵归位后才能检测
 * 如果泵不归位就扫描，则拒绝执行
 */
RetCode APhotoelectric::execSet_Detect() {
    auto dataUnit = create_modbusData(2, 1004);
    dataUnit.setValue(0, m_params.capacity);
    dataUnit.setValue(1, m_params.capacity >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    int  val = motorStep_to_microlitre_BSubstrate(m_params.capacity);
    emit Event::instance().signal_consume(DeviceConsumeType::BSubstrate, val);
    return exec_risingEdge(1000, 1);
}

RetCode APhotoelectric::execCheck_Detect() {
    auto state = Get_mcuStateBits();
    if (state.u1001.b00_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1001.b01_success == 1) {
        emit this->signal_DetectValue(Get_value());
        return RetCode::Success;
    }

    if (state.u1001.b02_detectError == 1) {
        return RetCode::DetectError;
    }
    return RetCode::UnknownError;
}

/**
 * 泵归位时，需要放一个空管进去防止泵有水
 */
RetCode APhotoelectric::execSet_HomingPump() {
    return exec_risingEdge(1000, 2);
}

RetCode APhotoelectric::execCheck_HomingPump() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1001.b00_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1001.b01_success == 1) {
        return RetCode::Success;
    }

    return RetCode::UnknownError;
}

RetCode APhotoelectric::execSet_OnlyDetect() {
    return exec_risingEdge(1000, 3);
}

RetCode APhotoelectric::execCheck_OnlyDetect() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1001.b00_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1001.b01_success == 1) {
        emit this->signal_DetectValue(Get_value());
        return RetCode::Success;
    }

    if (state.u1001.b02_detectError == 1) {
        return RetCode::DetectError;
    }
    return RetCode::UnknownError;
}

RetCode APhotoelectric::execSet_OnlyInjection() {
    auto dataUnit = create_modbusData(2, 1004);
    dataUnit.setValue(0, m_params.capacity);
    dataUnit.setValue(1, m_params.capacity >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    int  val = motorStep_to_microlitre_BSubstrate(m_params.capacity);
    emit Event::instance().signal_consume(DeviceConsumeType::BSubstrate, val);
    return exec_risingEdge(1000, 4);
}

RetCode APhotoelectric::execCheck_OnlyInjection() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1001.b00_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1001.b01_success == 1) {
        return RetCode::Success;
    }

    return RetCode::UnknownError;
}

RetCode APhotoelectric::execSet_HomingDoor() {
    return exec_risingEdge(1000, 5);
}

RetCode APhotoelectric::execCheck_HomingDoor() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1001.b00_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1001.b01_success == 1) {
        return RetCode::Success;
    }

    return RetCode::UnknownError;
}

RetCode APhotoelectric::execSet_DebugDoor() {
    auto dataUnit = create_modbusData(2, 1008);
    dataUnit.setValue(0, m_params.position);
    dataUnit.setValue(1, m_params.position >> 16u);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_risingEdge(1000, 6);
}

RetCode APhotoelectric::execCheck_DebugDoor() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1001.b00_leisure == 0) {
        return RetCode::Running;
    }
    if (state.u1001.b01_success == 1) {
        return RetCode::Success;
    }

    return RetCode::UnknownError;
}

RetCode APhotoelectric::execSet_RecoveryError() {
    return exec_risingEdge(1000, 15);
}

RetCode APhotoelectric::execCheck_RecoveryError() {
    return RetCode::Success;
}

}  // namespace COM

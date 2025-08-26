#include "AMixing.h"

#include "../../utility/PDT.h"
#include "../../utility/ToString.h"
#include "../DeviceManager.h"

namespace COM {

AMixing::AMixing(QObject *parent) : IDeviceThread(parent) {
    DataUnitConfig wconfig;
    wconfig.dataStartAddress = DeviceStartAddress;
    wconfig.deviceAddress    = DeviceAddress_Mixing;
    set_dataUnitConfig(wconfig);

    QVector<AtomicTask> setFun;
    setFun.push_back(std::bind(&AMixing::execSet_None, this));
    setFun.push_back(std::bind(&AMixing::execSet_EnableLoadMaterial, this));
    setFun.push_back(std::bind(&AMixing::execSet_HomingLoadMaterial, this));
    setFun.push_back(std::bind(&AMixing::execSet_EnableDivideCup, this));
    setFun.push_back(std::bind(&AMixing::execSet_HomingDivideCup, this));
    setFun.push_back(std::bind(&AMixing::execSet_EnableMix, this));
    setFun.push_back(std::bind(&AMixing::execSet_HomingMix, this));
    setFun.push_back(std::bind(&AMixing::execSet_DebugCup, this));
    setFun.push_back(std::bind(&AMixing::execSet_RecoveryError, this));

    QVector<AtomicTask> checkFun;
    checkFun.push_back(std::bind(&AMixing::execCheck_None, this));
    checkFun.push_back(std::bind(&AMixing::execCheck_EnableLoadMaterial, this));
    checkFun.push_back(std::bind(&AMixing::execCheck_HomingLoadMaterial, this));
    checkFun.push_back(std::bind(&AMixing::execCheck_EnableDivideCup, this));
    checkFun.push_back(std::bind(&AMixing::execCheck_HomingDivideCup, this));
    checkFun.push_back(std::bind(&AMixing::execCheck_EnableMix, this));
    checkFun.push_back(std::bind(&AMixing::execCheck_HomingMix, this));
    checkFun.push_back(std::bind(&AMixing::execCheck_DebugCup, this));
    checkFun.push_back(std::bind(&AMixing::execCheck_RecoveryError, this));

    assert(setFun.size() == checkFun.size());
    assert(setFun.size() == Mode_Count);
    set_loopFun(setFun, checkFun);
}

QString AMixing::to_string() const {
    QString msg = ">>>" + objectName() + "信息: \n";

    msg += Get_mcuVersion() + "\n";
    msg += "Bits: " + Get_mcuStateBits().to_string();
    msg += "Params: " + Get_mcuParams().to_string();

    msg += ">>>" + objectName();
    return msg;
}

QString AMixing::Get_mcuVersion() const {
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

//获取mixing的状态位
AMixing::StateBits AMixing::Get_mcuStateBits() const {
    QModbusDataUnit dataUnit = create_modbusData(8);  // 读mixing设备的8个寄存器值
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    StateBits stateBits;
    stateBits.isValid                      = reply.is_valid();
    stateBits.u1000_Mixing.ALL             = dataUnit.value(0);
    stateBits.u1001_Mixing.ALL             = dataUnit.value(1);
    stateBits.u1003_LoadMaterial_LMoto.ALL = dataUnit.value(3);
    stateBits.u1005_DivideCup_PMoto.ALL    = dataUnit.value(5);
    stateBits.u1007_Mix_VMoto.ALL          = dataUnit.value(7);
    return stateBits;
}

AMixing::Params AMixing::Get_mcuParams() const {
    QModbusDataUnit dataUnit = create_modbusData(14);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    ModbusArrayProxyView data(dataUnit, get_dataUnitConfig().dataStartAddress);
    Params               params;
    params.isValid        = reply.is_valid();
    params.cupTarPosition = twou16_to_oneu32<uint32_t>(data[1008], data[1009]);
    params.cupInPosition  = twou16_to_oneu32<uint32_t>(data[1010], data[1011]);
    params.cupOutPosition = twou16_to_oneu32<uint32_t>(data[1012], data[1013]);

    return params;
}

AMixing::ErrorPkg AMixing::Check_mcuError() const {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return {ErrorCode::SCModbusError, "no valid"};
    }
    return state.get_errorString();
}

/**
 * @brief TMixing::exec_risingEdge
 * @param startAddress  起始地址
 * @param bitAddress    bit的目标位置
 * 单个u16的某一个bit的上升沿操作
 */
RetCode AMixing::exec_risingEdge(int startAddress, int bitAddress) {
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

RetCode AMixing::execSet_None() {
    return RetCode::Success;
}

RetCode AMixing::execCheck_None() {
    return RetCode::Success;
}

RetCode AMixing::execSet_EnableLoadMaterial() {
    int r = rand() % 6;
    return exec_risingEdge(1002, r);
}

RetCode AMixing::execCheck_EnableLoadMaterial() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1003_LoadMaterial_LMoto.b00_leisure == 0) {
        return RetCode::Running;
    }
    return RetCode::Success;
}

RetCode AMixing::execSet_HomingLoadMaterial() {
    return exec_risingEdge(1002, 7);
}

RetCode AMixing::execCheck_HomingLoadMaterial() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1003_LoadMaterial_LMoto.b00_leisure == 0) {
        return RetCode::Running;
    }
    return RetCode::Success;
}

RetCode AMixing::execSet_EnableDivideCup() {
    return exec_risingEdge(1004, 0);
}

RetCode AMixing::execCheck_EnableDivideCup() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1005_DivideCup_PMoto.b00_leisure == 0) {
        return RetCode::Running;
    }
    return RetCode::Success;
}

/**
 * 分杯校准下位机未处理
 */
RetCode AMixing::execSet_HomingDivideCup() {
    return exec_risingEdge(1004, 7);
}

/**
 * 分杯校准下位机未处理
 */
RetCode AMixing::execCheck_HomingDivideCup() {
    return RetCode::Success;

    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1005_DivideCup_PMoto.b00_leisure == 0) {
        return RetCode::Running;
    }
    return RetCode::Success;
}

RetCode AMixing::execSet_EnableMix() {
    return exec_risingEdge(1006, 0);
}

RetCode AMixing::execCheck_EnableMix() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1007_Mix_VMoto.b00_leisure == 0) {
        return RetCode::Running;
    }
    return RetCode::Success;
}

RetCode AMixing::execSet_HomingMix() {
    return exec_risingEdge(1006, 7);
}

RetCode AMixing::execCheck_HomingMix() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1007_Mix_VMoto.b00_leisure == 0) {
        return RetCode::Running;
    }
    return RetCode::Success;
}

RetCode AMixing::execSet_DebugCup() {
    AMixing::Params sendParams = m_params;
    QModbusDataUnit dataUnit   = create_modbusData(2, 1008);
    uint16_t        low, high;

    std::tie(low, high) = oneu32_to_twou16(sendParams.cupTarPosition);
    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);

    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    }

    return exec_risingEdge(1004, 1);
}

RetCode AMixing::execCheck_DebugCup() {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return RetCode::ReadError;
    }
    if (state.u1005_DivideCup_PMoto.b00_leisure == 0) {
        return RetCode::Running;
    }
    return RetCode::Success;
}

RetCode AMixing::execSet_RecoveryError() {
    auto recovery = [this](int address) -> RetCode {
        QModbusDataUnit dataUnit = create_modbusData(1, address);

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
    };

    if (auto ret = recovery(1002); ret != RetCode::Success) {
        return ret;
    }
    if (auto ret = recovery(1004); ret != RetCode::Success) {
        return ret;
    }
    if (auto ret = recovery(1006); ret != RetCode::Success) {
        return ret;
    }

    return RetCode::Success;
}

RetCode AMixing::execCheck_RecoveryError() {
    return RetCode::Success;
}

}  // namespace COM

#include "AThermostat.h"

#include "../../config/CKeyJson.hpp"
#include "../../utility/PDT.h"
#include "../DeviceManager.h"

namespace COM {

AThermostat::AThermostat(QObject *parent) : IDeviceThread(parent) {
    DataUnitConfig wconfig;
    wconfig.dataStartAddress = 1000;
    wconfig.deviceAddress    = DeviceAddress_Thermostat;
    set_dataUnitConfig(wconfig);

    QVector<AtomicTask> setFun;
    setFun.push_back(std::bind(&AThermostat::execSet_None, this));
    setFun.push_back(std::bind(&AThermostat::execSet_PhotoelectricAble, this));
    setFun.push_back(std::bind(&AThermostat::execSet_IncubateAble, this));
    setFun.push_back(std::bind(&AThermostat::execSet_ReagentBinLidAble, this));
    setFun.push_back(std::bind(&AThermostat::execSet_ReagentBinAble, this));
    setFun.push_back(std::bind(&AThermostat::execSet_Fan0Able, this));
    setFun.push_back(std::bind(&AThermostat::execSet_Fan1Able, this));
    setFun.push_back(std::bind(&AThermostat::execSet_Fan2Able, this));
    setFun.push_back(std::bind(&AThermostat::execSet_Fan3Able, this));

    QVector<AtomicTask> checkFun;
    checkFun.push_back(std::bind(&AThermostat::execCheck_None, this));
    checkFun.push_back(std::bind(&AThermostat::execCheck_PhotoelectricAble, this));
    checkFun.push_back(std::bind(&AThermostat::execCheck_IncubateAble, this));
    checkFun.push_back(std::bind(&AThermostat::execCheck_ReagentBinLidAble, this));
    checkFun.push_back(std::bind(&AThermostat::execCheck_ReagentBinAble, this));
    checkFun.push_back(std::bind(&AThermostat::execCheck_Fan0Able, this));
    checkFun.push_back(std::bind(&AThermostat::execCheck_Fan1Able, this));
    checkFun.push_back(std::bind(&AThermostat::execCheck_Fan2Able, this));
    checkFun.push_back(std::bind(&AThermostat::execCheck_Fan3Able, this));

    assert(setFun.size() == checkFun.size());
    assert(setFun.size() == Mode_Count);
    set_loopFun(setFun, checkFun);
}

QString AThermostat::to_string() const {
    QString msg = ">>>" + objectName() + "信息: \n";

    msg += Get_mcuVersion() + "\n";
    msg += "Bits: " + Get_mcuStateBits().to_string();
    msg += "Params: " + Get_mcuParams().to_string();

    msg += ">>>" + objectName();
    return msg;
}

QString AThermostat::Get_mcuVersion() const {
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

AThermostat::StateBits AThermostat::Get_mcuStateBits() const {
    QModbusDataUnit dataUnit = create_modbusData(64);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    ModbusArrayProxyView data(dataUnit, get_dataUnitConfig().dataStartAddress);
    StateBits            state;
    state.isValid = reply.is_valid();

    state.u1002_photoelectric.ALL = data[1002];
    state.u1007_thermal.ALL       = data[1007];
    state.u1014_reagentBinLid.ALL = data[1014];
    state.u1020_reagentBin.ALL    = data[1020];

    return state;
}

AThermostat::Params AThermostat::Get_mcuParams() const {
    QModbusDataUnit dataUnit = create_modbusData(64);
    auto            reply    = read(dataUnit);
    dataUnit                 = reply.data;

    ModbusArrayProxyView data(dataUnit, get_dataUnitConfig().dataStartAddress);
    Params               params = m_params;
    params.isValid              = reply.is_valid();

    /// 光电仓
    params.photoelectricAble     = get_integerBitBool(data[1000], 0);
    params.photoelectricTemp     = twou16_to_oneu32<float>(data[1003], data[1004]);
    params.photoelectricElectric = twou16_to_oneu32<float>(data[1005], data[1006]);

    /// 温裕盘
    params.incubateAble     = get_integerBitBool(data[1000], 1);
    params.incubateTemp     = twou16_to_oneu32<float>(data[1008], data[1009]);
    params.incubateElectric = twou16_to_oneu32<float>(data[1010], data[1011]);

    /// 试剂仓盖
    params.reagentBinLidAble     = get_integerBitBool(data[1000], 2);
    params.reagentBinLidTemp     = twou16_to_oneu32<float>(data[1015], data[1016]);
    params.reagentBinLidElectric = twou16_to_oneu32<float>(data[1017], data[1018]);

    /// 试剂仓
    params.reagentBinAble             = get_integerBitBool(data[1000], 3);
    params.reagentBinTemp             = twou16_to_oneu32<float>(data[1021], data[1022]);
    params.reagentBinPeltierFrontTemp = twou16_to_oneu32<float>(data[1023], data[1024]);
    params.reagentBinPeltierBackTemp  = twou16_to_oneu32<float>(data[1025], data[1026]);
    params.reagentBinPEL1Electric     = twou16_to_oneu32<float>(data[1027], data[1028]);
    params.reagentBinPEL2Electric     = twou16_to_oneu32<float>(data[1029], data[1030]);
    params.reagentBinOutElectric      = twou16_to_oneu32<float>(data[1031], data[1032]);
    /// 试剂仓的风扇
    params.fan0Able                   = get_integerBitBool(data[1019], 0);
    params.fan1Able                   = get_integerBitBool(data[1019], 1);
    params.fan2Able                   = get_integerBitBool(data[1019], 2);
    params.fan3Able                   = get_integerBitBool(data[1019], 3);

    return params;
}

AThermostat::ErrorPkg AThermostat::Check_mcuError() const {
    auto state = Get_mcuStateBits();
    if (state.isValid == false) {
        return {ErrorCode::SCModbusError, "no valid"};
    }
    return state.get_errorString();
}

/**
 * 可以多状态运行，只改变其中的一个bit
 */
RetCode AThermostat::set_onceBitState(int startAddress, unsigned bitIdx, int flag) {
    QModbusDataUnit dataUnit = create_modbusData(1, startAddress);
    auto            reply    = read(dataUnit);
    if (reply.is_valid() == false) {
        return RetCode::ReadError;
    }
    dataUnit = reply.data;

    uint16_t val = dataUnit.value(0);
    val          = set_integerBit(val, bitIdx, flag);
    dataUnit.setValue(0, val);
    if (!write(dataUnit).is_valid()) {
        return RetCode::WriteError;
    } else {
        return RetCode::Success;
    }
}

RetCode AThermostat::execSet_None() {
    return RetCode::Success;
}

RetCode AThermostat::execCheck_None() {
    return RetCode::Success;
}

RetCode AThermostat::execSet_PhotoelectricAble() {
    QModbusDataUnit dataUnit = create_modbusData(2, 1035);
    uint16_t        low, high;
    std::tie(low, high) = oneu32_to_twou16(m_params.photoelectricTemp);

    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    auto reply = write(dataUnit);
    if (reply.is_valid() == false) {
        return RetCode::WriteError;
    }
    return set_onceBitState(1000, 0, m_params.photoelectricAble);
}

RetCode AThermostat::execCheck_PhotoelectricAble() {
    return RetCode::Success;
}

RetCode AThermostat::execSet_IncubateAble() {
    QModbusDataUnit dataUnit = create_modbusData(2, 1012);
    uint16_t        low, high;
    std::tie(low, high) = oneu32_to_twou16(m_params.incubateTemp);

    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    auto reply = write(dataUnit);
    if (reply.is_valid() == false) {
        return RetCode::WriteError;
    }
    return set_onceBitState(1000, 1, m_params.incubateAble);
}

RetCode AThermostat::execCheck_IncubateAble() {
    return RetCode::Success;
}

RetCode AThermostat::execSet_ReagentBinLidAble() {
    QModbusDataUnit dataUnit = create_modbusData(2, 1033);
    uint16_t        low, high;
    std::tie(low, high) = oneu32_to_twou16(m_params.reagentBinLidTemp);

    dataUnit.setValue(0, low);
    dataUnit.setValue(1, high);
    auto reply = write(dataUnit);
    if (reply.is_valid() == false) {
        return RetCode::WriteError;
    }

    return set_onceBitState(1000, 2, m_params.reagentBinLidAble);
}

RetCode AThermostat::execCheck_ReagentBinLidAble() {
    return RetCode::Success;
}

RetCode AThermostat::execSet_ReagentBinAble() {
    return set_onceBitState(1000, 3, m_params.reagentBinAble);
}

RetCode AThermostat::execCheck_ReagentBinAble() {
    return RetCode::Success;
}

RetCode AThermostat::execSet_Fan0Able() {
    return set_onceBitState(1019, 0, m_params.fan0Able);
}

RetCode AThermostat::execCheck_Fan0Able() {
    return RetCode::Success;
}

RetCode AThermostat::execSet_Fan1Able() {
    return set_onceBitState(1019, 1, m_params.fan1Able);
}

RetCode AThermostat::execCheck_Fan1Able() {
    return RetCode::Success;
}

RetCode AThermostat::execSet_Fan2Able() {
    return set_onceBitState(1019, 2, m_params.fan2Able);
}

RetCode AThermostat::execCheck_Fan2Able() {
    return RetCode::Success;
}

RetCode AThermostat::execSet_Fan3Able() {
    return set_onceBitState(1019, 3, m_params.fan3Able);
}

RetCode AThermostat::execCheck_Fan3Able() {
    return RetCode::Success;
}

}  // namespace COM

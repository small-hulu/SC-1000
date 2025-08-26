#include "IModbusProxy.h"

namespace COM {
IModbusProxy::IModbusProxy() {
    m_modbusView = &ModbusImpl::instance();
}

IModbusProxy::~IModbusProxy() {}

void IModbusProxy::set_dataUnitConfig(IModbusProxy::DataUnitConfig config) {
    m_dataUnitConfig = config;
}

IModbusProxy::DataUnitConfig IModbusProxy::get_dataUnitConfig() const {
    return m_dataUnitConfig;
}

QModbusDataUnit IModbusProxy::create_modbusData(int len) const {
    return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, m_dataUnitConfig.dataStartAddress,
                           len);
}

QModbusDataUnit IModbusProxy::create_modbusData(int len, int startAddress) const {
    return QModbusDataUnit(QModbusDataUnit::HoldingRegisters, startAddress, len);
}

}  // namespace COM

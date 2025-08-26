#ifndef ICOM_H_1710303155
#define ICOM_H_1710303155

#include <QDebug>
#include <QString>
#include <QThread>

#include "../device/ModbusImpl.h"

namespace COM {

class IModbusProxy {
protected:
    struct DataUnitConfig {
        int dataStartAddress;  // 数据起始地址
        int deviceAddress;     // 设备地址

        DataUnitConfig(int start = 1000, int device = 0)
            : dataStartAddress(start), deviceAddress(device) {}
    };

protected:
    ModbusImpl*    m_modbusView = nullptr;
    DataUnitConfig m_dataUnitConfig;

public:
    IModbusProxy();
    virtual ~IModbusProxy();

public:
    virtual QString to_string() const = 0;

public:
    void           set_dataUnitConfig(DataUnitConfig config);
    DataUnitConfig get_dataUnitConfig() const;
    /// 使用set的config的地址
    QModbusDataUnit create_modbusData(int len) const;
    QModbusDataUnit create_modbusData(int len, int startAddress) const;

public:
    ModbusReplyWrapper read(const QModbusDataUnit& data) const {
        return m_modbusView->read(m_dataUnitConfig.deviceAddress, data);
    }
    ModbusReplyWrapper write(const QModbusDataUnit& data) const {
        return m_modbusView->write(m_dataUnitConfig.deviceAddress, data);
    }
};

/**
 * @brief The ModbusArrayProxyView class
 * Help-Data
 * 根据当前协议，进行位置的偏移
 * 当前协议一般是从1000开始的
 * 而这里封装的modbus是[0, n)的形式
 * 因此要用data[i - 1000]的形式
 */
class ModbusArrayProxyView {
    QModbusDataUnit& data;
    int              offset = 1000;

public:
    ModbusArrayProxyView(QModbusDataUnit& data, int offset = 1000) : data(data), offset(offset) {}

    uint16_t operator[](int idx) const {
        assert(idx >= offset);
        /// 这里返回的是值而不是引用，很尴尬
        return data.value(idx - offset);
    }
};

}  // namespace COM

#endif  // ICOM_H_1710303155

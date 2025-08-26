#ifndef UTOSTRING_H_1713843179
#define UTOSTRING_H_1713843179

#include <QModbusDataUnit>
#include <QSerialPortInfo>

namespace COM {
/**
 * 要用这里的`to_string`全部需要特化
 */
template <typename T>
QString to_string(T) = delete;

QString to_string(const QModbusDataUnit& data);

QString to_string(const QSerialPortInfo& info);

};  // namespace COM

#endif  // UTOSTRING_H_1713843179

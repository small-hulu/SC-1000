#include "Qrcode_SerialBase.h"

#include <QtDebug>

namespace QRCODE {
SerialBase::SerialBase(QObject *parent) : QObject(parent) {
    { Reset(); }

    /// connect
    {
        connect(&m_serial, &QSerialPort::errorOccurred, this, &SerialBase::handle_errorOccurred);

        connect(&m_serial, &QSerialPort::readyRead, this, &SerialBase::handle_readyRead);
    }
}

void SerialBase::Reset() {
    if (m_serial.isOpen()) {
        m_serial.close();
    }
    m_serial.setBaudRate(QSerialPort::Baud115200);
    m_serial.setStopBits(QSerialPort::OneStop);
    m_serial.setParity(QSerialPort::NoParity);
}

/**
 * 是否打开
 */
bool SerialBase::isOpen() {
    return m_serial.isOpen();
}

/**
 * 每次打开，若已打开则先关闭
 * 需要reset一次整体状态
 */
bool SerialBase::open() {
    /// 由于串口的操作都是非原子性的，这里设置为最高优先级
    /// 原先有开放的必须关闭重新设置
    if (isOpen()) {
        close();
    }

    Reset();
    m_serial.setPortName(m_info.port);
    /// 只读
    if (false == m_serial.open(QIODevice::ReadOnly)) {
        qWarning() << "serial open failed | portName " << m_info.port;
        return false;
    }
    return true;
}

/**
 * 注意，colse本质是一个阻塞的函数
 */
bool SerialBase::close() {
    m_serial.close();
    return true;
}
}  // namespace QRCODE

#include "SerialBase.h"

namespace LIS {
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
    m_serial.setPortName(m_info.port_name);
    m_serial.setBaudRate(m_info.BaudRate);
    m_serial.setDataBits(m_info.DataBits);
    m_serial.setParity(m_info.Checkbit);
    m_serial.setStopBits(m_info.Stopbit);
    m_serial.setFlowControl(m_info.FlowControl);
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

    /// 读写
    if (false == m_serial.open(QIODevice::ReadWrite)) {
        qWarning() << objectName() << "open failed" << m_info.port_name;
        return false;
    } else {
        qInfo() << objectName() << "open success" << m_info.port_name;
        return true;
    }
}

/**
 * 注意，colse本质是一个阻塞的函数
 */
bool SerialBase::close() {
    m_serial.close();
    return true;
}

void SerialBase::Write_bySequenced(const QByteArray &data) {
    if (isOpen()) {
        m_serial.write(data);
        if (!m_serial.waitForBytesWritten(1000)) {
            qWarning() << objectName() << "Failed to write data to serial port";
        }
    } else {
        qWarning() << objectName() << "Serial port is not open";
    }
}

void SerialBase::Write_byParallel(const QByteArray &data) {
    if (isOpen()) {
        m_serial.write(data);
    } else {
        qWarning() << objectName() << "Serial port is not open";
    }
}

}  // namespace LIS

#include "Qrcode_SerialQrcode.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QTemporaryFile>

namespace QRCODE {
SerialQrcode::SerialQrcode(QObject *parent) : SerialBase(parent) {
    Reset();
}

int SerialQrcode::Start_periodicEvents(int ms) {
    return m_timeId = this->startTimer(ms, Qt::TimerType::VeryCoarseTimer);
}

void SerialQrcode::Stop_periodicEvents() {
    this->killTimer(m_timeId);
    m_timeId = -1;
}

void SerialQrcode::Reset() {
    SerialBase::Reset();
    m_serialRawData.clear();
    m_lastAnalysisedData.clear();
}

void SerialQrcode::handle_errorOccurred(QSerialPort::SerialPortError error) {
    /// 经过测试，当串口线拔出的时候，会发生这个（ResourceError）错误
    if (error == QSerialPort::SerialPortError::ResourceError) {
        this->close();
    }
    qWarning() << error;
}

void SerialQrcode::handle_readyRead() {
    m_serialRawData += m_serial.readAll();

try_again:
    if (m_serialRawData.size() < 4) {
        return;
    }

    union {
        uint8_t odata[4];
        int32_t length = 0;
    } info;
    info.odata[0] = m_serialRawData[3];
    info.odata[1] = m_serialRawData[2];
    info.odata[2] = m_serialRawData[1];
    info.odata[3] = m_serialRawData[0];

    if (m_serialRawData.size() < info.length + 4 + 1) {
        return;
    }

    /// 注意，扫描枪的尾部有一个后缀
    m_lastAnalysisedData = qUncompress(m_serialRawData.mid(0, info.length + 4));
    m_serialRawData.remove(0, info.length + 4 + 1);
    const int type = analyse_codeType(m_lastAnalysisedData);
    emit      signal_qrcode_originalData(m_lastAnalysisedData, type);

    goto try_again;
}

int SerialQrcode::analyse_codeType(const QByteArray &bytes) {
    qInfo().noquote().nospace() << bytes;
    int type = 0;

    QTemporaryFile file;
    if (file.open()) {
        file.write(bytes);
        file.flush();

        /// 基于条码规定特化
        const QString typeKey = "base/codetype";
        QSettings     settings(file.fileName(), QSettings::IniFormat);
        type = settings.value(typeKey).toInt();
    }

    sLog_qrcode() << QString("检验条码类型[%1]").arg(type);
    return type;
}

void SerialQrcode::timerEvent(QTimerEvent *event) {
    if (m_serial.isOpen()) {
        return;
    }

    for (QSerialPortInfo info : QSerialPortInfo::availablePorts()) {
        if (info.vendorIdentifier() == m_info.vid) {
            m_info.port = info.portName();
            bool ok     = this->open();
            sLog_qrcode() << QString("扫描枪尝试连接[%1]").arg(info.portName()) << ok;
            break;
        }
    }

    QObject::timerEvent(event);
}
}  // namespace QRCODE

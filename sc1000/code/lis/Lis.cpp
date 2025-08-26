#include "Lis.h"

#include <QDebug>
#include <QSerialPortInfo>
#include <QTemporaryFile>

namespace LIS {
Lis::Lis(QObject* parent) : SerialBase(parent) {
    setObjectName("lis");
    Reset();
}

void Lis::Reset() {
    SerialBase::Reset();
    m_serialRawData.clear();
    m_lastAnalysisedData.clear();
}

void Lis::handle_errorOccurred(QSerialPort::SerialPortError error) {
    if (error == QSerialPort::SerialPortError::ResourceError) {
        this->close();
    }
    qWarning() << objectName() << error;
}

void Lis::handle_readyRead() {
    m_serialRawData += m_serial.readAll();
    while (true) {
        auto&& s = m_serialRawData;

        const char* start = "pmd";
        const char* end   = "\r\n";
        if (s.contains(start) && s.contains(end)) {
            s.remove(0, s.indexOf(start));
        } else {
            break;
        }
        auto sub = s.mid(0, s.indexOf(end) + QString(end).size());
        s.remove(0, s.indexOf(end) + QString(end).size());
        m_lastAnalysisedData = sub;
        qDebug() << objectName() << "mode" << (int)m_mode << "parse bytes" << sub;
        if (m_mode == Mode::Bidirectional) {
            emit signal_lisData(sub);
        }
    }

    qDebug() << objectName() << "reveice surplus bytes" << m_serialRawData;
}
}  // namespace LIS

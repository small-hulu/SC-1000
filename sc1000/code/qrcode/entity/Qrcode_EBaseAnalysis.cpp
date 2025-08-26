#include "Qrcode_EBaseAnalysis.h"

#include <QDebug>
#include <QTemporaryFile>

namespace QRCODE {
EBaseAnalysis::EBaseAnalysis() {
    Reset({});
}

EBaseAnalysis::EBaseAnalysis(const QByteArray &bytes) {
    Reset(bytes);
}

QString EBaseAnalysis::to_string() const {
    QString msg;
    QDebug  qos(&msg);
    qos << m_map;
    return msg;
}

QString EBaseAnalysis::list_to_string(const QStringList &stringList) const {
    QString msg;
    QDebug  qos(&msg);
    qos << stringList;
    return msg;
}
void EBaseAnalysis::Reset(const QByteArray &bytes) {
    m_bytes = bytes;
    m_map.clear();

    QTemporaryFile file;
    if (file.open()) {
        file.write(bytes);
        file.flush();
    }
    QSettings ini(file.fileName(), QSettings::IniFormat);
    for (auto &&key : ini.allKeys()) {
        m_map[key] = ini.value(key);
    }
}
}  // namespace QRCODE

#include "Qrcode_EConsumable.h"

#include <QTemporaryFile>

namespace QRCODE {

int EConsumable::Get_consumableType() {
    int type = 0;

    QTemporaryFile file;
    if (file.open()) {
        file.write(m_bytes);
        file.flush();

        /// 基于条码规定特化
        const QString typeKey = KeyCon::base_consty;
        QSettings     settings(file.fileName(), QSettings::IniFormat);
        type = settings.value(typeKey).toInt();
    }
    return type;
}

}  // namespace QRCODE

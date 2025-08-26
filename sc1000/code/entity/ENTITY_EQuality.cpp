#include "ENTITY_EQuality.h"

namespace ENTITY {

QList<QString> EQuality::Get_projectList() {
    const attr key = attr::project;
    return table[key].split(join_separator, QString::SkipEmptyParts);
}

QDateTime EQuality::Get_expirationDate() {
    namespace qrattr = QRCODE::KeyQua;
    auto qr          = Get_qrcodeEntity();
    return QDateTime::fromString(qr[qrattr::base_producdate].toString(), QRCODE::time_format)
        .addDays(qr[qrattr::base_validity].toInt());
}

}  // namespace ENTITY

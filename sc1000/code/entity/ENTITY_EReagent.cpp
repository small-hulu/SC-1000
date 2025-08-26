#include "ENTITY_EReagent.h"

#include "qrcode/QRCODE.hpp"

namespace ENTITY {
QString EReagent::Get_Project() {
    return {table[attr::project]};
}

/**
 * 只增不减
 */
void EReagent::Add_measureCnt() {
    int cnt                    = table[attr::measuredTimes].toInt();
    table[attr::measuredTimes] = QString::number(cnt + 1);
}

/**
 * 根据批号得出瓶号
 */
QString EReagent::Get_bottleNumber() {
    QRCODE::EReagent qrcode(table[attr::qrcode].toUtf8());
    namespace attr_qr = QRCODE::KeyRea;
    return qrcode[attr_qr::base_bottleno].toString();
}

/**
 * 有一个不为0即可
 */
QString EReagent::Get_MainCurve() {
    auto x = table[attr::A];
    for (auto key : {attr::A, attr::B, attr::C, attr::D}) {
        if (table[key].isEmpty() == false) {
            return QString("已导入");
        }
    }
    return QString("未导入");
}

std::optional<QColor> EReagent::Deter_surplus(QString data) {
    auto qrcode   = Get_qrcodeEntity();
    namespace key = QRCODE::KeyRea;
    double val    = qrcode[key::base_tq].toString().toDouble();
    double diff   = data.toDouble();

    if (diff <= val * 0.1) {
        return QColor(Qt::red);
    } else if (diff <= val * 0.2) {
        return QColor(255, 144, 12);
    }
    return std::nullopt;
}

}  // namespace ENTITY

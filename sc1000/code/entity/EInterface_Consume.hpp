#ifndef EINTERFACE_CONSUME_HPP
#define EINTERFACE_CONSUME_HPP

#include <QDebug>

#include "EInterface_DB.hpp"
#include "qrcode/QRCODE.hpp"

namespace ENTITY {
struct EInterface_Consume : public EInterface_DB<::DB::TableEntity::consume> {
public:
    static constexpr double Min_Rate = 0.05;

public:
    virtual ~EInterface_Consume() = default;

public:
    virtual void Reset() {
        table[attr::time]    = "";
        table[attr::usedVal] = QString::number(0);
        table[attr::remark]  = "";
        table[attr::qrcode]  = "";
    }

public:
    QRCODE::EConsumable Get_qrcodeEntity() {
        QRCODE::EConsumable qr(table[attr::qrcode].toUtf8());
        return qr;
    }

    virtual QString Get_consumeName() const {
        return table[attr::type];
    }

    virtual QList<QString> Get_descriptorMessage() {
        return {};
    }

    virtual QString Get_hintMessage() {
        return "todo";
    }

public:
    /**
     * 累计的消耗
     */
    virtual void Cumulative_consumption(double val) {
        double savedVal       = table[attr::usedVal].toDouble();
        savedVal             += val;
        table[attr::usedVal]  = QString::number(savedVal);
        Update_toDB();
    }

    /**
     * 装载时的量 - 消耗的量
     */
    virtual double Get_surplusCapacity() {
        auto   qrcode   = Get_qrcodeEntity();
        double baseVal  = qrcode[QRCODE::KeyCon::base_packspec].toDouble() * 1000;
        double savedVal = table[attr::usedVal].toDouble();
        return baseVal - savedVal;
    }

    /**
     * 获取剩余的比例
     * 有些耗材的不用比例，直接 0/1
     */
    virtual double Get_ratio() {
        auto   qrcode     = Get_qrcodeEntity();
        double baseVal    = qrcode[QRCODE::KeyCon::base_packspec].toDouble() * 1000;
        double savedVal   = table[attr::usedVal].toDouble();
        double surplusVal = qMax(0.0, baseVal - savedVal);
        return surplusVal / baseVal;
    }
};
}  // namespace ENTITY

#endif  // EINTERFACE_CONSUME_HPP

#ifndef CONSUMEBSUBSTRATE_H_1725416549
#define CONSUMEBSUBSTRATE_H_1725416549

#include "EInterface_Consume.hpp"
#include "EInterface_DB.hpp"

namespace ENTITY {
class ConsumeBSubstrate : public EInterface_Consume {
public:
    ConsumeBSubstrate() = default;

public:
    QString Get_consumeName() const override {
        return "激发液B";
    }

    QList<QString> Get_descriptorMessage() override {
        auto           qrcode = Get_qrcodeEntity();
        QList<QString> ret;
        ret << QString("装载日期");
        ret << table[attr::time];
        ret << QString("有效天数");
        ret << qrcode[QRCODE::KeyCon::base_validity].toString();
        ret << QString("生产日期");
        ret << qrcode[QRCODE::KeyCon::base_producdate].toString();
        ret << QString("批号");
        ret << qrcode[QRCODE::KeyCon::base_batchnum].toString();
        ret << QString("瓶号");
        ret << qrcode[QRCODE::KeyCon::base_bottleno].toString();
        return ret;
    }

public:
    virtual QString Get_hintMessage() override {
        double val = Get_surplusCapacity();
        return QString::number(val, 'f', 0) + QString("µl");
    }
};
}  // namespace ENTITY

#endif  // CONSUMEBSUBSTRATE_H_1725416549

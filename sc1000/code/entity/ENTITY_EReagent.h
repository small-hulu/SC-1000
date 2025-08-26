/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       ENTITY_EReagent.h
 * @date       2024-06-07
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      1539349804@qq.com
 *
 * @brief
 * 一个实体类设计的几项约束
 * 每一个属性，均和数据库中的属性对应
 * 属性一般分为两类
 * 1. 前端展示
 * 2. 后端辅助
 *  + 主键
 *  + option标记
 *  + 等等
 *
 * 成员数据的类型
 * 1. 内置类型，请在申明的时候写入初始值
 * 2. 类类型，可默认
 *****************************************************************************/
#ifndef EREAGENT_H_1715765527
#define EREAGENT_H_1715765527

#include "EInterface_DB.hpp"
#include "algorithm/algorithm.hpp"
#include "qrcode/QRCODE.hpp"

namespace ENTITY {
class EReagent : public EInterface_DB<::DB::TableEntity::reagent> {
public:
    struct ConfigExpProcess {
        /// 试剂仓组号
        int idxBinGroup = 0;
    };

public:
    EReagent() = default;

    virtual std::size_t Get_hash() override {
        auto hash = UTILITY::get_hash(id, qHash(table[attr::qrcode]));
        return UTILITY::get_hash(hash, reinterpret_cast<std::size_t>(this));
    }

public:
    QRCODE::EReagent Get_qrcodeEntity() {
        QRCODE::EReagent qr(table[attr::qrcode].toUtf8());
        return qr;
    }

public:
    QString Get_Project();
    void    Add_measureCnt();
    QString Get_bottleNumber();
    QString Get_MainCurve();

    FourPLC Get_4plc() {
        FourPLC plc;
        plc.A = table[attr::A].toDouble();
        plc.B = table[attr::B].toDouble();
        plc.C = table[attr::C].toDouble();
        plc.D = table[attr::D].toDouble();
        return plc;
    }
    void Set_4plc(FourPLC plc) {
        table[attr::A] = QString::number(plc.A);
        table[attr::B] = QString::number(plc.B);
        table[attr::C] = QString::number(plc.C);
        table[attr::D] = QString::number(plc.D);
    }
    void set_liquidConsume(int hole, int val) {
        QString     liquidConsume = table[attr::liquidConsume];
        QStringList consumeList   = liquidConsume.split(",", QString::SkipEmptyParts);
        if (hole >= 0 && hole < consumeList.size()) {
            bool ok;
            int  currentValue = consumeList[hole].toInt(&ok);
            if (ok) {
                currentValue      += val;
                consumeList[hole]  = QString::number(currentValue);
            }
        }
        table[attr::liquidConsume] = consumeList.join(",");
    }

    std::optional<QColor> Deter_surplus(QString data);
};

}  // namespace ENTITY
#endif  // EREAGENT_H_1715765527

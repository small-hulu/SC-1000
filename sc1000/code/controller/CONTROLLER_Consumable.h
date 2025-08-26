#ifndef CONSUMABLE_H_1722849817
#define CONSUMABLE_H_1722849817

#include <QObject>

#include "entity/ENTITY_ConsumeASubstrate.h"
#include "entity/ENTITY_ConsumeBSubstrate.h"
#include "entity/ENTITY_ConsumeReactionCup.h"
#include "entity/ENTITY_ConsumeWashLiquid.h"
#include "entity/ENTITY_ConsumeWashLiquidEnhance.h"
#include "entity/ENTITY_ConsumeWasteLiquidTank.h"
#include "entity/ENTITY_ConsumeWasteTank.h"

namespace CONTROLLER {
class Consumable : QObject {
    Q_OBJECT
public:
    using Self = Consumable;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

public:
    using entity_attr = ENTITY::EInterface_Consume::attr;

public:
    enum ConsumableType {
        ASubstrat,  /// 底物A
        BSubstrat,  /// 底物B
        WasteTank,  /// 废料箱
    };
    Q_ENUM(ConsumableType);

public:
    ENTITY::ConsumeASubstrate cons_ASubstrat;
    ENTITY::ConsumeBSubstrate cons_BSubstrat;
    ENTITY::WasteLiquidTank   cons_wasteLiquidTank;
    ENTITY::WasteTank         cons_wasteTank;
    ENTITY::ReactionCup       cons_reactionCup;
    ENTITY::WashLiquid        cons_washLiquid;
    ENTITY::WashLiquidEnhance cons_washLiquidEnhance;

public:
    Consumable();

public:
    /// 获取 激发液 简单信息字符串
    QString Get_describe_for_Substrat();
    /// 获取 废料箱 简单信息字符串
    QString Get_describe_for_WasteTank();

public:
    /// 处理条码扫描信息
    void Qrcode_for_Consumable(const QByteArray& bytes, int type);
    /// 处理上传来的耗材信息
    void Com_for_Consumable(int type, double reduceVal);
};
}  // namespace CONTROLLER

#endif  // CONSUMABLE_H_1722849817

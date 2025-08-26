#include "ENTITY_ConsumeWasteTank.h"

namespace ENTITY {

QString WasteTank::Get_hintMessage() {
    const int use = table[attr::usedVal].toInt();
    return QString("已累计 %1").arg(use);
}

double WasteTank::Get_ratio() {
    const int use = table[attr::usedVal].toInt();
    if (use >= MaxCup) {
        return 0;
    }
    return 1.0 * (MaxCup - use) / MaxCup;
}
}  // namespace ENTITY

#include "CONTROLLER_Calibration.h"

namespace CONTROLLER {

Calibration::entity_item Calibration::Filter_by_batchNum(const QString& num) {
    for (auto&& item : Select_all_fromDB()) {
        if (num == item->table[entity_attr::batchNum]) {
            return item;
        }
    }
    return {};
}

QList<QString> Calibration::Filter_allBatchNum() {
    QList<QString> res;
    for (auto&& item : Select_all_fromDB()) {
        res << item->table[entity_attr::batchNum];
    }
    return res;
}
}  // namespace CONTROLLER

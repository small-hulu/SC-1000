#ifndef CALIBRATION_H_1719972093
#define CALIBRATION_H_1719972093

#include "CInterface_DBEntity.hpp"
#include "entity/ENTITY_ECalibration.h"

namespace CONTROLLER {
class Calibration : public CInterface_DBEntity< ::ENTITY::ECalibration> {
public:
    using Self = Calibration;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

private:
    Calibration() = default;

public:
    /// 通过批号筛选出
    entity_item Filter_by_batchNum(const QString& num);

public:
    /// 选出所有批号
    QList<QString> Filter_allBatchNum();
};
}  // namespace CONTROLLER

#endif  // CALIBRATION_H_1719972093

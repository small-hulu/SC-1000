#ifndef EXPCALIBRATION_H_1720579825
#define EXPCALIBRATION_H_1720579825

#include "CInterface_DBEntity.hpp"
#include "CONTROLLER_Calibration.h"
#include "CONTROLLER_Reagent.h"
#include "entity/ENTITY_EExpCalibration.h"

namespace CONTROLLER {
class ExpCalibration : public CInterface_DBEntity<::ENTITY::EExpCalibration> {
public:
    using Self = ExpCalibration;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

public:
    ExpCalibration() = default;

public:
    QList<entity_item> Get_theList_expNotDone() {
        return Select_byAttribute_isEqual(entity_attr::state,
                                          QString::number((int)Type::ETubeState::Apply));
    }

    QList<entity_item> Get_theList_expDone() {
        return Select_byAttribute_isNotEqual(entity_attr::state,
                                             QString::number((int)Type::ETubeState::Apply));
    }
};
}  // namespace CONTROLLER

#endif  // EXPCALIBRATION_H_1720579825

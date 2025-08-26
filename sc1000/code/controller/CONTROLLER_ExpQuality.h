#ifndef EXPQUALITY_H_1721286741
#define EXPQUALITY_H_1721286741

#include "CInterface_DBEntity.hpp"
#include "CONTROLLER_Quality.h"
#include "entity/ENTITY_EExpQuality.h"

namespace CONTROLLER {
class ExpQuality : public CInterface_DBEntity< ::ENTITY::EExpQuality> {
public:
    using Self = ExpQuality;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

public:
    ExpQuality() = default;

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
#endif  // EXPQUALITY_H

#ifndef EXPERIMENT_H_1719971051
#define EXPERIMENT_H_1719971051

#include "CInterface_DBEntity.hpp"
#include "CONTROLLER_Reagent.h"
#include "CONTROLLER_Sample.h"
#include "entity/ENTITY_EExpSample.h"

namespace CONTROLLER {

class ExpSample : public CInterface_DBEntity<::ENTITY::EExpSample> {
public:
    using Self = ExpSample;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

private:
    ExpSample() = default;
};
}  // namespace CONTROLLER
#endif  // EXPERIMENT_H_1719971051

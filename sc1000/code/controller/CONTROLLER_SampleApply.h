#ifndef SAMPLEAPPLY_H_1731402130
#define SAMPLEAPPLY_H_1731402130

#include "CInterface_DBEntity.hpp"
#include "CONTROLLER_Sample.h"
#include "entity/ENTITY_ESampleApply.h"

namespace CONTROLLER {
class SampleApply : public CInterface_DBEntity<::ENTITY::ESampleApply> {
public:
    using Self = SampleApply;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

public:
    SampleApply() = default;
};
}  // namespace CONTROLLER
#endif  // SAMPLEAPPLY_H_1731402130

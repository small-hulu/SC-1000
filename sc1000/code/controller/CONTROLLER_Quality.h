#ifndef QUALITY_H_1719972192
#define QUALITY_H_1719972192

#include "CInterface_DBEntity.hpp"
#include "entity/ENTITY_EQuality.h"

namespace CONTROLLER {
class Quality : public CInterface_DBEntity< ::ENTITY::EQuality> {
public:
    using Self = Quality;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

private:
    Quality() = default;
};
}  // namespace CONTROLLER
#endif  // QUALITY_H_1719972192

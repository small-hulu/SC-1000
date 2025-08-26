#ifndef REAGENTBINSTATE_H_1727244711
#define REAGENTBINSTATE_H_1727244711

#include "CONTROLLER_ExpCalibration.h"
#include "CONTROLLER_ExpQuality.h"
#include "CONTROLLER_ExpSample.h"
#include "global/GLOBAL_TypeTube.h"

namespace CONTROLLER {
class ReagentBinState {
public:
    using Self = ReagentBinState;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

public:
    struct HoleInfo {};

private:
    QHash<Type::ETubeSpaceMark, HoleInfo> table;

private:
    ReagentBinState() = default;
};
}  // namespace CONTROLLER

#endif  // REAGENTBINSTATE_H_1727244711

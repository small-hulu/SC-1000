#ifndef PIPETTENEEDLEDILUTE_H_1723776500
#define PIPETTENEEDLEDILUTE_H_1723776500

#include "PipetteNeedle.h"

namespace COM::WFIXTURE {
class PipetteNeedleDilute : public PipetteNeedle {
public:
    PipetteNeedleDilute();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE

#endif  // PIPETTENEEDLEDILUTE_H_1723776500

#ifndef PIPETTENEEDLEWASH_H_1723512313
#define PIPETTENEEDLEWASH_H_1723512313

#include "PipetteNeedle.h"

namespace COM::WFIXTURE {
class PipetteNeedleWash : public PipetteNeedle {
public:
    PipetteNeedleWash();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE

#endif  // PIPETTENEEDLEWASH_H_1723512313

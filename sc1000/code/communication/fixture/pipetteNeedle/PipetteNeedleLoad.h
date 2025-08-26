#ifndef PIPETTENEEDLEWASH_H_1723512313_1723513424
#define PIPETTENEEDLEWASH_H_1723512313_1723513424

#include "PipetteNeedle.h"

namespace COM::WFIXTURE {
class PipetteNeedleLoad : public PipetteNeedle {
public:
    PipetteNeedleLoad();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE

#endif  // PIPETTENEEDLEWASH_H_1723512313_1723513424

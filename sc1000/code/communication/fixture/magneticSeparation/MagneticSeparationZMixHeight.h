#ifndef MAGNETICSEPARATIONZMIXHEIGHT_H_1725263027
#define MAGNETICSEPARATIONZMIXHEIGHT_H_1725263027

#include "MagneticSeparation.h"

namespace COM::WFIXTURE {
class MagneticSeparationZMixHeight : public MagneticSeparation {
public:
    MagneticSeparationZMixHeight();

public:
    bool Save_fixtureParams() override;

protected:
    void exec_fixtureMove() override;
};
}  // namespace COM::WFIXTURE

#endif  // MAGNETICSEPARATIONZMIXHEIGHT_H_1725263027

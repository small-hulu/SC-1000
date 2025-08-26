#ifndef MAGNETICSEPARATIONZDRAINDEEP_H_1725262567
#define MAGNETICSEPARATIONZDRAINDEEP_H_1725262567

#include "MagneticSeparation.h"

namespace COM::WFIXTURE {
class MagneticSeparationZDrainDeep : public MagneticSeparation {
public:
    MagneticSeparationZDrainDeep();

public:
    bool Save_fixtureParams() override;

protected:
    void exec_fixtureMove() override;
};
}  // namespace COM::WFIXTURE

#endif  // MAGNETICSEPARATIONZDRAINDEEP_H_1725262567

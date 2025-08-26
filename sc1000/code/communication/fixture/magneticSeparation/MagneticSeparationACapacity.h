#ifndef MAGNETICSEPARATIONACAPACITY_H_1723529254
#define MAGNETICSEPARATIONACAPACITY_H_1723529254

#include "MagneticSeparation.h"

namespace COM::WFIXTURE {
class MagneticSeparationACapacity : public MagneticSeparation {
public:
    MagneticSeparationACapacity();

public:
    bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE

#endif  // MAGNETICSEPARATIONACAPACITY_H_1723529254

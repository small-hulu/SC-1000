#ifndef GRIPPERMAGNETICSEPARATION_H_1723453498
#define GRIPPERMAGNETICSEPARATION_H_1723453498

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperMagneticSeparation : public Gripper {
public:
    GripperMagneticSeparation();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE
#endif  // GRIPPERMAGNETICSEPARATION_H_1723453498

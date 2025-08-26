#ifndef GRIPPERTHROW_H_1723454160
#define GRIPPERTHROW_H_1723454160
#include "Gripper.h"
namespace COM::WFIXTURE {
class GripperThrow : public Gripper {
public:
    GripperThrow();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERTHROW_H_1723454160

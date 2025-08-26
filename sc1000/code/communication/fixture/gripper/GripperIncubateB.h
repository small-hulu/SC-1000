#ifndef GRIPPERINCUBATEB_H_1725341171
#define GRIPPERINCUBATEB_H_1725341171

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperIncubateB : public Gripper {
public:
    GripperIncubateB();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE
#endif  // GRIPPERINCUBATEB_H_1725341171

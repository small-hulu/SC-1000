#ifndef GRIPPERINCUBATEA_H_1725341009
#define GRIPPERINCUBATEA_H_1725341009

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperIncubateA : public Gripper {
public:
    GripperIncubateA();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERINCUBATEA_H_1725341009

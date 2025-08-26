#ifndef GRIPPERMIXING_H_1723453708
#define GRIPPERMIXING_H_1723453708

#include "Gripper.h"

namespace COM::WFIXTURE {

class GripperMixing : public Gripper {
public:
    GripperMixing();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERMIXING_H

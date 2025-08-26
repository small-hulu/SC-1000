#ifndef GRIPPERCUP_H_1723452300
#define GRIPPERCUP_H_1723452300

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperCup : public Gripper {
public:
    GripperCup();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERCUP_H_1723452300

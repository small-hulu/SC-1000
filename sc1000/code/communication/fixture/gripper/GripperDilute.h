#ifndef GRIPPERDILUTE_H_1723775722
#define GRIPPERDILUTE_H_1723775722

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperDilute : public Gripper {
public:
    GripperDilute();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;

};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERDILUTE_H_1723775722

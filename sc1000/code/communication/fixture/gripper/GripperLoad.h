#ifndef GRIPPERLOAD_H_1723452592
#define GRIPPERLOAD_H_1723452592

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperLoad : public Gripper {
public:
    GripperLoad();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;

};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERLOAD_H_1723452592

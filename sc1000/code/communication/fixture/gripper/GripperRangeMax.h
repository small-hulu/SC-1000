#ifndef GRIPPERRANGEMAX_H_1725255388
#define GRIPPERRANGEMAX_H_1725255388

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperRangeMax : public Gripper {
public:
    GripperRangeMax();

public:
    bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERRANGEMAX_H_1725255388

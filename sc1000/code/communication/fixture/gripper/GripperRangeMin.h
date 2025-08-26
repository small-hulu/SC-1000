#ifndef GRIPPERRANGEMIN_H_1725255388
#define GRIPPERRANGEMIN_H_1725255388

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperRangeMin : public Gripper {
public:
    GripperRangeMin();

public:
    bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERRANGEMIN_H_1725255388

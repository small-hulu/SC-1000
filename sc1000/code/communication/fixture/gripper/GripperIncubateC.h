#ifndef GRIPPERINCUBATEC_H_1725341293
#define GRIPPERINCUBATEC_H_1725341293

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperIncubateC : public Gripper {
public:
    GripperIncubateC();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE
#endif  // GRIPPERINCUBATEC_H_1725341293

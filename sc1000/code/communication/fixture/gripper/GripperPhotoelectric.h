#ifndef GRIPPERPHOTOELECTRIC_H_1723454030
#define GRIPPERPHOTOELECTRIC_H_1723454030

#include "Gripper.h"

namespace COM::WFIXTURE {
class GripperPhotoelectric : public Gripper {
public:
    GripperPhotoelectric();
    ~GripperPhotoelectric();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;
};
}  // namespace COM::WFIXTURE

#endif  // GRIPPERPHOTOELECTRIC_H_1723454030

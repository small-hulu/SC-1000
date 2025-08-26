#ifndef GRIPPER_H_723443086
#define GRIPPER_H_723443086

#include "../WOperationPanel.h"
#include "communication/device/gripper/DGripper.h"
#include "communication/utility/JsonConfig.h"

namespace COM::WFIXTURE {
class Gripper : public OperationPanel {
protected:
    using Device = DGripper;
    Device::Params m_params;
    Device::Params o_params;
public:
    Gripper();
    ~Gripper();

protected:
    void exec_fixtureMove() override;
    void exec_fixtureMove_Z() override;
protected:
    void op_xadd() override;
    void op_xsub() override;
    void op_yadd() override;
    void op_ysub() override;
    void op_zadd() override;
    void op_zsub() override;
    void op_clockwise() override;
    void op_anticlockwise() override;
};
}  // namespace COM::WFIXTURE
#endif  // GRIPPER_H_723443086

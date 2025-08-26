#ifndef FIXMIXING_H_1725264060
#define FIXMIXING_H_1725264060

#include "../WOperationPanel.h"
#include "communication/device/mixing/DMixing.h"

namespace COM::WFIXTURE {
class FixCup : public OperationPanel {
protected:
    using Device = DMixing;
    DMixing::Params m_params;

public:
    FixCup();
    ~FixCup();

protected:
    void op_yadd() override;
    void op_ysub() override;

protected:
    void exec_fixtureMove() override;

protected:
    void homing();
};
}  // namespace COM::WFIXTURE

#endif  // FIXMIXING_H_1725264060

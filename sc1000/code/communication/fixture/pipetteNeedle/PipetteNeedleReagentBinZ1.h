#ifndef PipetteNeedleReagentBinZ1Z1_H_1733721734
#define PipetteNeedleReagentBinZ1Z1_H_1733721734

#include "PipetteNeedle.h"
#include "communication/device/reagentBin/DReagentBin.h"

namespace COM::WFIXTURE {

template <size_t BinIdx>
class PipetteNeedleReagentBinZ1 : public PipetteNeedle {
private:
    DReagentBin::Params reagent_params;

public:
    PipetteNeedleReagentBinZ1();

public:
    bool Save_fixtureParams() override;
    bool Save_fixtureParams_Without_Z() override;

private:
    void exec_fixtureMove_ReagentBin() {
        auto&& device = DReagentBin::instance();
        device.Set_Config(DReagentBin::Mode_Position, reagent_params);
        device.Exec_sequencedTask();
    }

private:
    void op_clockwise() override;
    void op_anticlockwise() override;
};
}  // namespace COM::WFIXTURE

#endif  // PipetteNeedleReagentBinZ1Z1_H_1733721734

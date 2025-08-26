#ifndef REAGENTBIN_H_1723520343
#define REAGENTBIN_H_1723520343

#include "../WOperationPanel.h"
#include "communication/config/CKeyJson.hpp"
#include "communication/device/reagentBin/DReagentBin.h"
#include "communication/utility/JsonConfig.h"

namespace COM::WFIXTURE {
class ReagentBin : public OperationPanel {
protected:
    using Device = DReagentBin;
    Device::Params m_params;

public:
    ReagentBin();
    ~ReagentBin();

protected:
    void exec_fixtureMove() override;

private:
    void op_clockwise() override;
    void op_anticlockwise() override;
};
}  // namespace COM::WFIXTURE

#endif  // REAGENTBIN_H_1723520343

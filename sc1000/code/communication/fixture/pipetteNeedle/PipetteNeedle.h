#ifndef PIPETTENEEDLE_H_1723511611
#define PIPETTENEEDLE_H_1723511611

#include "../WOperationPanel.h"
#include "communication/device/pipettingNeedle/DPipetteNeedle.h"
#include "communication/utility/JsonConfig.h"

namespace COM::WFIXTURE {
class PipetteNeedle : public OperationPanel {
protected:
    using Device = DPipetteNeedle;
    Device::Params m_params;
    Device::Params o_params;

public:
    PipetteNeedle();
    ~PipetteNeedle();

protected:
    void exec_fixtureMove() override;
    void exec_fixtureMove_Z() override;
private:
    void op_yadd() override;
    void op_ysub() override;
    void op_zadd() override;
    void op_zsub() override;
};
}  // namespace COM::WFIXTURE

#endif  // PIPETTENEEDLE_H_1723511611

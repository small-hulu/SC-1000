#ifndef MAGNETICSEPARATION_H_1723527407
#define MAGNETICSEPARATION_H_1723527407

#include "../WOperationPanel.h"
#include "communication/device/magneticSeparation/DMagneticSeparation.h"
#include "communication/utility/JsonConfig.h"

namespace COM::WFIXTURE {
class MagneticSeparation : public OperationPanel {
protected:
    using Device = DMagneticSeparation;
    Device::Params m_params;

public:
    MagneticSeparation();
    ~MagneticSeparation();

protected:
    void op_zadd() override;
    void op_zsub() override;
    void op_clockwise() override;
    void op_anticlockwise() override;
};
}  // namespace COM::WFIXTURE

#endif  // MAGNETICSEPARATION_H_1723527407

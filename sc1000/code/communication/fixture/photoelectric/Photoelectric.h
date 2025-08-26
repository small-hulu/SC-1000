#ifndef PHOTOELECTRIC_H_1723530440
#define PHOTOELECTRIC_H_1723530440

#include "../WOperationPanel.h"
#include "communication/device/photoelectric/DPhotoelectric.h"
#include "communication/utility/JsonConfig.h"

namespace COM::WFIXTURE {
class Photoelectric : public OperationPanel {
protected:
    using Device = DPhotoelectric;
    Device::Params m_params;

public:
    Photoelectric();
    ~Photoelectric();

protected:
    void op_yadd() override;
    void op_ysub() override;
};
}  // namespace COM::WFIXTURE

#endif  // PHOTOELECTRIC_H_1723530440

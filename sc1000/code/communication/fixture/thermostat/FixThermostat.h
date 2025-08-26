#ifndef FIXTHERMOSTAT_H_1725269185
#define FIXTHERMOSTAT_H_1725269185

#include "../WOperationPanel.h"
#include "communication/device/thermostat/DThermostat.h"

namespace COM::WFIXTURE {
class FixThermostat : public OperationPanel {
protected:
    using Device = DThermostat;
    Device::Params m_params;

public:
    FixThermostat();
};
}  // namespace COM::WFIXTURE

#endif  // FIXTHERMOSTAT_H_1725269185

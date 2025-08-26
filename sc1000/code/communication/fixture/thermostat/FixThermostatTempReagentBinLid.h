#ifndef FIXTHERMOSTATTEMPREAGENTBINLID_H_1725329489
#define FIXTHERMOSTATTEMPREAGENTBINLID_H_1725329489

#include "FixThermostat.h"

namespace COM::WFIXTURE {
class FixThermostatTempReagentBinLid : public FixThermostat {
public:
    FixThermostatTempReagentBinLid();

public:
    virtual bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE
#endif  // FIXTHERMOSTATTEMPREAGENTBINLID_H_1725329489

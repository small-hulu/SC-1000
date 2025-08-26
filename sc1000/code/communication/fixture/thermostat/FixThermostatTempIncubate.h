#ifndef FIXTHERMOSTATTEMPINCUBATE_H_1725329327
#define FIXTHERMOSTATTEMPINCUBATE_H_1725329327

#include "FixThermostat.h"

namespace COM::WFIXTURE {
class FixThermostatTempIncubate : public FixThermostat {
public:
    FixThermostatTempIncubate();

public:
    virtual bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE

#endif  // FIXTHERMOSTATTEMPINCUBATE_H_1725329327

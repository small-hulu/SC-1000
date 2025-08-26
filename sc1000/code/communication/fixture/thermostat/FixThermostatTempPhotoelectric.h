#ifndef FIXTHERMOSTATTEMPPHOTOELECTRIC_H_1725328977
#define FIXTHERMOSTATTEMPPHOTOELECTRIC_H_1725328977

#include "FixThermostat.h"

namespace COM::WFIXTURE {
class FixThermostatTempPhotoelectric : public FixThermostat {
public:
    FixThermostatTempPhotoelectric();

public:
    virtual bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE

#endif  // FIXTHERMOSTATTEMPPHOTOELECTRIC_H_1725328977

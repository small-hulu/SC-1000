#ifndef MAGNETICSEPARATIONTRAY_H_1723527692
#define MAGNETICSEPARATIONTRAY_H_1723527692

#include "MagneticSeparation.h"

namespace COM::WFIXTURE {
class MagneticSeparationTray : public MagneticSeparation {
public:
    MagneticSeparationTray();

public:
    bool Save_fixtureParams() override;

protected:
    void exec_fixtureMove() override;
};
}  // namespace COM::WFIXTURE

#endif  // MAGNETICSEPARATIONTRAY_H_1723527692

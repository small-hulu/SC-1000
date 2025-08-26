#ifndef REAGENTBINBACK_H_1723520743
#define REAGENTBINBACK_H_1723520743

#include "ReagentBin.h"

namespace COM::WFIXTURE {

template <size_t BinIdx>
class ReagentBinBack : public ReagentBin {
public:
    ReagentBinBack();

public:
    bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE
#endif  // REAGENTBINBACK_H_1723520743

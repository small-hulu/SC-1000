#ifndef REAGENTBINFront_H_1723526520C
#define REAGENTBINFront_H_1723526520C

#include "ReagentBin.h"

namespace COM::WFIXTURE {

template <size_t BinIdx>
class ReagentBinFront : public ReagentBin {
public:
    ReagentBinFront();

public:
    bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE
#endif  // REAGENTBINFront_H_1723526520C

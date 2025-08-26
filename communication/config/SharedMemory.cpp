#include "SharedMemory.h"

namespace COM {

void SharedMemory::Set_ReagentBinHole_liquidResidual(int idxBinGroup, int idxBinBack, int val) {
    assert(idxBinGroup >= 0 && idxBinGroup < ReagentBinBinCount);
    assert(idxBinBack >= 0 && idxBinBack < ReagentBinHoleReagentCount);
    m_ReagentBinSet[idxBinGroup].liquidResidualList[idxBinBack] = val;
}

void SharedMemory::Set_ReagentBinHole_liquidResidual(int          idxBinGroup,
                                                     QVector<int> liquidResidualList) {
    for (int i = 0; i < ReagentBinHoleReagentCount; ++i) {
        if (i < liquidResidualList.size()) {
            Set_ReagentBinHole_liquidResidual(idxBinGroup, i, liquidResidualList[i]);
        }
    }
}

int SharedMemory::Get_ReagentBinHole_liquidResidual(int idxBinGroup, int idxBinBack) {
    assert(idxBinGroup >= 0 && idxBinGroup < ReagentBinBinCount);
    assert(idxBinBack >= 0 && idxBinBack < ReagentBinHoleReagentCount);
    return m_ReagentBinSet[idxBinGroup].liquidResidualList[idxBinBack];
}

}  // namespace COM

#include "ReagentBinBack.h"

namespace COM::WFIXTURE {

template class ReagentBinBack<0>;
template class ReagentBinBack<1>;
template class ReagentBinBack<2>;
template class ReagentBinBack<3>;
template class ReagentBinBack<4>;
template class ReagentBinBack<5>;
template class ReagentBinBack<6>;

template <size_t BinIdx>
ReagentBinBack<BinIdx>::ReagentBinBack() : ReagentBin() {
    auto&& device = Device::instance();
    m_params      = device.GetInfo_backHole(BinIdx);
    device.Set_Config(Device::Mode_Position, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

template <size_t BinIdx>
bool ReagentBinBack<BinIdx>::Save_fixtureParams() {
    auto&& device               = Device::instance();
    device.fix.backList[BinIdx] = m_params.position;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

#include "ReagentBinFront.h"

namespace COM::WFIXTURE {

template class ReagentBinFront<0>;
template class ReagentBinFront<1>;
template class ReagentBinFront<2>;
template class ReagentBinFront<3>;
template class ReagentBinFront<4>;
template class ReagentBinFront<5>;
template class ReagentBinFront<6>;

template <size_t BinIdx>
ReagentBinFront<BinIdx>::ReagentBinFront() : ReagentBin() {
    auto&& device = Device::instance();
    m_params      = device.GetInfo_frontHole(BinIdx);
    device.Set_Config(Device::Mode_Position, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

template <size_t BinIdx>
bool ReagentBinFront<BinIdx>::Save_fixtureParams() {
    auto&& device                = Device::instance();
    device.fix.frontList[BinIdx] = m_params.position;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

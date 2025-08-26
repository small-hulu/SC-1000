#include "MagneticSeparationTray.h"

namespace COM::WFIXTURE {
MagneticSeparationTray::MagneticSeparationTray() : MagneticSeparation() {
    auto&& device = Device::instance();
    m_params      = device.GetParams_to_Grapper();
    device.Set_Config(Device::Mode_AbleTrayMove, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

bool MagneticSeparationTray::Save_fixtureParams() {
    auto&& device = Device::instance();
    device.Calc_byGripper(m_params.position);

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}

void MagneticSeparationTray::exec_fixtureMove() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_AbleTrayMove, m_params);
    device.Exec_sequencedTask();
}
}  // namespace COM::WFIXTURE

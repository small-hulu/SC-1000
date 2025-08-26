#include "MagneticSeparationZDrainDeep.h"

namespace COM::WFIXTURE {
MagneticSeparationZDrainDeep::MagneticSeparationZDrainDeep() {
    OperationPanel::Sudoku matrx = {
        {0, 0, 1},  //
        {0, 0, 0},  //
        {1, 0, 0}   //
    };
    Set_Sudoku(matrx);

    auto&& device = Device::instance();
    m_params      = device.GetParams_to_Needle1();
    device.Set_Config(Device::Mode_AbleTrayMove, m_params);
    device.Exec_sequencedTask();

    m_params             = device.Get_mcuParams();
    m_params.zAxisHeight = m_params.drainDeep;
    device.Set_Config(Device::Mode_DebugZ, m_params);
    device.Exec_sequencedTask();
}

bool MagneticSeparationZDrainDeep::Save_fixtureParams() {
    auto&& device        = Device::instance();
    device.fix.drainDeep = m_params.zAxisHeight;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}

void MagneticSeparationZDrainDeep::exec_fixtureMove() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_DebugZ, m_params);
    device.Exec_sequencedTask();
}
}  // namespace COM::WFIXTURE

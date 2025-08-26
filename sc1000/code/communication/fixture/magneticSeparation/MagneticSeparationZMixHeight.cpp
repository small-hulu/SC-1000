#include "MagneticSeparationZMixHeight.h"

namespace COM::WFIXTURE {
MagneticSeparationZMixHeight::MagneticSeparationZMixHeight() {
    OperationPanel::Sudoku matrx = {
        {0, 0, 1},  //
        {0, 0, 0},  //
        {1, 0, 0}   //
    };
    Set_Sudoku(matrx);

    auto&& device = Device::instance();
    m_params      = device.GetParams_to_Wash();
    device.Set_Config(Device::Mode_AbleTrayMove, m_params);
    device.Exec_sequencedTask();
    m_params             = device.Get_mcuParams();
    m_params.zAxisHeight = m_params.mixHeight;
    device.Set_Config(Device::Mode_DebugZ, m_params);
    device.Exec_sequencedTask();
}

bool MagneticSeparationZMixHeight::Save_fixtureParams() {
    auto&& device        = Device::instance();
    device.fix.mixHeight = m_params.zAxisHeight;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}

void MagneticSeparationZMixHeight::exec_fixtureMove() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_DebugZ, m_params);
    device.Exec_sequencedTask();
}
}  // namespace COM::WFIXTURE

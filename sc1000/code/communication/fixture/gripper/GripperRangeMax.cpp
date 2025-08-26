#include "GripperRangeMax.h"

namespace COM::WFIXTURE {
GripperRangeMax::GripperRangeMax() {
    OperationPanel::Sudoku matrx = {
        {1, 0, 0},  //
        {0, 0, 0},  //
        {0, 0, 1}   //
    };
    Set_Sudoku(matrx);

    auto&& device      = Device::instance();
    m_params           = device.Get_mcuParams();
    m_params.openRange = m_params.rangeMax;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

bool GripperRangeMax::Save_fixtureParams() {
    auto&& device       = DGripper::instance();
    device.fix.rangeMax = m_params.openRange;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

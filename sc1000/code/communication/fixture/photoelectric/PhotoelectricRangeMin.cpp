#include "PhotoelectricRangeMin.h"

namespace COM::WFIXTURE {
PhotoelectricRangeMin::PhotoelectricRangeMin() {
    OperationPanel::Sudoku matrx = {
        {0, 1, 0},  //
        {0, 0, 0},  //
        {0, 1, 0}   //
    };
    Set_Sudoku(matrx);

    auto&& device     = DPhotoelectric::instance();
    m_params          = device.Get_mcuParams();
    m_params.position = m_params.positionIn;
    device.Set_Config(Device::Mode_DebugDoor, m_params);
    device.Exec_sequencedTask();
}

void PhotoelectricRangeMin::exec_fixtureMove() {
    auto&& device = DPhotoelectric::instance();
    device.Set_Config(Device::Mode_DebugDoor, m_params);
    device.Exec_sequencedTask();
}

bool PhotoelectricRangeMin::Save_fixtureParams() {
    auto&& device       = DPhotoelectric::instance();
    device.fix.rangeMin = m_params.position;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

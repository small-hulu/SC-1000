#include "ReagentBin.h"

namespace COM::WFIXTURE {
ReagentBin::ReagentBin() {
    OperationPanel::Sudoku matrx = {
        {1, 0, 0},  //
        {0, 0, 0},  //
        {0, 0, 1}   //
    };
    Set_Sudoku(matrx);

    /// 每次转动太耗时，目前先不考虑homing
    /*
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_Homing, m_params);
    device.Exec_sequencedTask();
    */
}

ReagentBin::~ReagentBin() {
    /// 每次转动太耗时，目前先不考虑homing
    /*
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_Homing, m_params);
    device.Exec_sequencedTask();
    */
}

void ReagentBin::exec_fixtureMove() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_Position, m_params);
    device.Exec_sequencedTask();
}

void ReagentBin::op_clockwise() {
    int num            = get_inputNum();
    m_params.position -= num;
    exec_fixtureMove();
}

void ReagentBin::op_anticlockwise() {
    int num            = get_inputNum();
    m_params.position += num;
    exec_fixtureMove();
}
}  // namespace COM::WFIXTURE

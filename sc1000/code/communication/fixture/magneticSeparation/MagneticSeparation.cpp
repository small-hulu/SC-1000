#include "MagneticSeparation.h"

namespace COM::WFIXTURE {
MagneticSeparation::MagneticSeparation() : OperationPanel() {
    OperationPanel::Sudoku matrx = {
        {1, 0, 0},  //
        {0, 0, 0},  //
        {0, 0, 1}   //
    };
    Set_Sudoku(matrx);

    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_HomingZ, m_params);
    device.Exec_sequencedTask();
    /*
    device.Set_Config(Device::Mode_HomingTray, m_params);
    device.Exec_sequencedTask();
    */
}

MagneticSeparation::~MagneticSeparation() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_HomingZ, m_params);
    device.Exec_sequencedTask();
    /*
    device.Set_Config(Device::Mode_HomingTray, m_params);
    device.Exec_sequencedTask();
    */
}

void MagneticSeparation::op_zadd() {
    int num               = get_inputNum();
    m_params.zAxisHeight += num;
    exec_fixtureMove();
}

void MagneticSeparation::op_zsub() {
    int num               = get_inputNum();
    m_params.zAxisHeight -= num;
    exec_fixtureMove();
}

void MagneticSeparation::op_clockwise() {
    int num            = get_inputNum();
    m_params.position += num;
    exec_fixtureMove();
}

void MagneticSeparation::op_anticlockwise() {
    int num            = get_inputNum();
    m_params.position -= num;
    exec_fixtureMove();
}
}  // namespace COM::WFIXTURE

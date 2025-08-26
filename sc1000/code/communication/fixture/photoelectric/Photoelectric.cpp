#include "Photoelectric.h"

namespace COM::WFIXTURE {
Photoelectric::Photoelectric() : OperationPanel() {
    OperationPanel::Sudoku matrx = {
        {0, 0, 0},  //
        {0, 0, 0},  //
        {0, 0, 0}   //
    };
    Set_Sudoku(matrx);

    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_HomingDoor, m_params);
    device.Exec_sequencedTask();
}

Photoelectric::~Photoelectric() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_HomingDoor, m_params);
    device.Exec_sequencedTask();
}

void Photoelectric::op_yadd() {
    int num            = get_inputNum();
    m_params.position += num;
    exec_fixtureMove();
}

void Photoelectric::op_ysub() {
    int num            = get_inputNum();
    m_params.position -= num;
    exec_fixtureMove();
}
}  // namespace COM::WFIXTURE

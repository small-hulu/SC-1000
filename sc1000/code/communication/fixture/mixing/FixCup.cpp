#include "FixCup.h"

namespace COM::WFIXTURE {
FixCup::FixCup() {
    OperationPanel::Sudoku matrx = {
        {0, 1, 0},  //
        {0, 0, 0},  //
        {0, 1, 0}   //
    };
    Set_Sudoku(matrx);

    homing();
}

FixCup::~FixCup() {
    homing();
}

void FixCup::op_yadd() {
    int num                  = get_inputNum();
    m_params.cupTarPosition += num;
    exec_fixtureMove();
}

void FixCup::op_ysub() {
    int num                  = get_inputNum();
    m_params.cupTarPosition -= num;
    exec_fixtureMove();
}

void FixCup::exec_fixtureMove() {
    auto&& device = DMixing::instance();
    device.Set_Config(Device::Mode_DebugCup, m_params);
    device.Exec_sequencedTask();
}

void FixCup::homing() {
    auto&& device = DMixing::instance();
    device.Set_Config(Device::Mode_HomingDivideCup, m_params);
    device.Exec_sequencedTask();
}
}  // namespace COM::WFIXTURE

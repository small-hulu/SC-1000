#include "PipetteNeedle.h"

namespace COM::WFIXTURE {
PipetteNeedle::PipetteNeedle() {
    OperationPanel::Sudoku matrx = {
        {0, 1, 1},  //
        {0, 0, 0},  //
        {1, 1, 0}   //
    };
    Set_Sudoku(matrx);

    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_Homing, m_params);
    device.Exec_sequencedTask();
}

PipetteNeedle::~PipetteNeedle() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_Homing, m_params);
    device.Exec_sequencedTask();
}

void PipetteNeedle::exec_fixtureMove() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_YPosition, m_params);
    device.Exec_sequencedTask();
}
void PipetteNeedle::exec_fixtureMove_Z() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_DebugYZPosition, m_params);
    device.Exec_sequencedTask();
}
void PipetteNeedle::op_yadd() {
    int num             = get_inputNum();
    m_params.yPosition += num;
    exec_fixtureMove_Z();
}

void PipetteNeedle::op_ysub() {
    int num             = get_inputNum();
    m_params.yPosition -= num;
    exec_fixtureMove_Z();
}

void PipetteNeedle::op_zadd() {
    int num             = get_inputNum();
    m_params.zPosition -= num;
    exec_fixtureMove_Z();
}

void PipetteNeedle::op_zsub() {
    int num             = get_inputNum();
    m_params.zPosition += num;
    exec_fixtureMove_Z();
}
}  // namespace COM::WFIXTURE

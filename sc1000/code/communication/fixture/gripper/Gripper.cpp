#include "Gripper.h"

#include <QDebug>

namespace COM::WFIXTURE {
Gripper::Gripper() {
    OperationPanel::Sudoku matrx = {
        {1, 1, 1},  //
        {1, 0, 1},  //
        {1, 1, 1}   //
    };
    Set_Sudoku(matrx);

    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_Homing, m_params);
    device.Exec_sequencedTask();
}

Gripper::~Gripper() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_Homing, m_params);
    device.Exec_sequencedTask();
}

void Gripper::exec_fixtureMove() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_XYPosition, m_params);
    device.Exec_sequencedTask();
}
void Gripper::exec_fixtureMove_Z() {
    auto&& device = Device::instance();
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
}
void Gripper::op_xadd() {
    int num             = get_inputNum();
    m_params.xPosition += num;
    exec_fixtureMove_Z();
}

void Gripper::op_xsub() {
    int num             = get_inputNum();
    m_params.xPosition -= num;
    exec_fixtureMove_Z();
}

void Gripper::op_yadd() {
    int num             = get_inputNum();
    m_params.yPosition -= num;
    exec_fixtureMove_Z();
}

void Gripper::op_ysub() {
    int num             = get_inputNum();
    m_params.yPosition += num;
    exec_fixtureMove_Z();
}

void Gripper::op_zadd() {
    int num             = get_inputNum();
    m_params.zPosition -= num;
    qDebug()<<"z_add"<<m_params.to_string()<<endl;
    exec_fixtureMove_Z();
}

void Gripper::op_zsub() {
    int num             = get_inputNum();
    qDebug()<<"z_add"<<m_params.to_string()<<endl;
    m_params.zPosition += num;
    exec_fixtureMove_Z();
}

void Gripper::op_clockwise() {
    int num             = get_inputNum();
    m_params.openRange += num;
    exec_fixtureMove_Z();
}

void Gripper::op_anticlockwise() {
    int num             = get_inputNum();
    m_params.openRange -= num;
    exec_fixtureMove_Z();
}
}  // namespace COM::WFIXTURE

#include "GripperCup.h"

#include "communication/device/mixing/DMixing.h"

namespace COM::WFIXTURE {
GripperCup::GripperCup() : Gripper() {
    auto&& cup = DMixing::instance();
    cup.Set_Config(DMixing::Mode_HomingDivideCup, {});
    cup.Exec_sequencedTask();
    cup.Set_Config(DMixing::Mode_EnableDivideCup, {});
    cup.Exec_parallelTask();

    auto&& device      = Device::instance();
    m_params           = device.GetInfo_to_Cup().params;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

bool GripperCup::Save_fixtureParams() {
    auto&& device  = DGripper::instance();
    qDebug()<<m_params.to_string();
    device.fix.Cup = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
///在Z不动的情况下仅存XY的位置
bool GripperCup::Save_fixtureParams_Without_Z() {
    auto&& device  = DGripper::instance();
    m_params.zPosition = o_params.zPosition;
    device.fix.Cup = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}

}  // namespace COM::WFIXTURE

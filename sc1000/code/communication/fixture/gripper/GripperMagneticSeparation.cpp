#include "GripperMagneticSeparation.h"

#include "communication/device/magneticSeparation/DMagneticSeparation.h"

namespace COM::WFIXTURE {
GripperMagneticSeparation::GripperMagneticSeparation() : Gripper() {
    auto&&                      mag = DMagneticSeparation::instance();
    DMagneticSeparation::Params magParams;
    mag.Set_Config(DMagneticSeparation::Mode_HomingZ, magParams);
    mag.Exec_sequencedTask();
    mag.Set_Config(DMagneticSeparation::Mode_HomingTray, magParams);
    mag.Exec_sequencedTask();
    magParams = mag.GetParams_to_Grapper();
    mag.Set_Config(DMagneticSeparation::Mode_AbleTrayMove, magParams);
    mag.Exec_sequencedTask();

    auto&& device      = Device::instance();
    m_params           = device.GetInfo_to_MagneticSeparation().params;
    o_params           = device.GetInfo_to_Cup().params;
    m_params.zPosition = 0;
    device.Set_Config(Device::Mode_XYZPosition, m_params);
    device.Exec_sequencedTask();
    m_params = device.Get_mcuParams();
}

bool GripperMagneticSeparation::Save_fixtureParams() {
    auto&& device                 = DGripper::instance();
    device.fix.MagneticSeparation = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}

bool GripperMagneticSeparation::Save_fixtureParams_Without_Z() {
    auto&& device                 = DGripper::instance();
    m_params.zPosition = o_params.zPosition;
    device.fix.MagneticSeparation = m_params;

    device.Save_fileFixtureParams();
    device.Reset_cacheFixtureParams();
    device.Init_muc();
    return true;
}
}  // namespace COM::WFIXTURE

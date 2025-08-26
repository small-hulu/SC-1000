#ifndef GRIPPER_H_1704183100
#define GRIPPER_H_1704183100

#include "../IDeviceConfig.hpp"
#include "../IDeviceThread.h"
#include "../IModbusProxy.h"
#include "CGripper.h"

namespace COM {
class AGripper
    : public IDeviceThread
    , public IModbusProxy
    , public IDeviceConfig<CGripper> {
    Q_OBJECT
public:
    enum Mode : uint16_t {
        Mode_None        = 0x00,  /// 无(保留模式)
        Mode_XYPosition  = 0x01,  /// xy运动模式
        Mode_Grab        = 0x02,  /// 夹取模式
        Mode_Release     = 0x03,  /// 释放模式
        Mode_HardBrake   = 0x04,  /// 急刹车
        Mode_Homing      = 0x05,  /// 回原点(校准)
        Mode_XYZPosition = 0x06,  /// 回原点(校准)
        Mode_MoveGrab    = 0x07,  /// 移动并夹取
        Mode_MoveRelease = 0x08,  /// 移动并释放
        Mode_RecoveryError,       /// 恢复错误

        Mode_Count  /// 此处有两个作用，1)枚举计数 2)非法标志
    };
    Q_ENUM(Mode)

public:
    explicit AGripper(QObject *parent = nullptr);

public:
    QString to_string() const override;

public:
    void Set_Config(int mode, const Params &params) override {
        m_params    = params;
        m_modeIndex = mode;
    }

    QString   Get_mcuVersion() const override;
    StateBits Get_mcuStateBits() const override;
    Params    Get_mcuParams() const override;
    ErrorPkg  Check_mcuError() const override;

private:
    RetCode solve_mcuError();

    RetCode set_modeType(Mode mode);
    void    exec_check_modeReady();
    /// 运动不用上升沿，下位机会先检测到模式后自己set
    RetCode exec_setUpExec();

private:
    RetCode execSet_None();
    RetCode execCheck_None();

    RetCode execSet_XYPosition();
    RetCode execCheck_XYPosition();

    RetCode execSet_Grab();
    RetCode execCheck_Grab();

    RetCode execSet_Release();
    RetCode execCheck_Release();

    RetCode execSet_HardBrake();
    RetCode execCheck_HardBrake();

    RetCode execSet_Homing();
    RetCode execCheck_Homing();

    RetCode execSet_XYZPosition();
    RetCode execCheck_XYZPosition();

    RetCode execSet_MoveGrab();
    RetCode execCheck_MoveGrab();

    RetCode execSet_MoveRelease();
    RetCode execCheck_MoveRelease();

    RetCode execSet_RecoveryError();
    RetCode execCheck_RecoveryError();
};
}  // namespace COM

#endif  // GRIPPER_H_1704183100

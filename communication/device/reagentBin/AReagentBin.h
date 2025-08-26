/******************************************************************************
 * Copyright © 2023 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       MODBUS_ReagentPlate.h
 * @brief      试剂盘的操作
 * set各种配置后，进行run
 *
 * @author     luyinfeng
 * @Email      luyinfeng@smtbio.com
 *
 * @date       2023-12-26
 * @version    0.0.0
 *****************************************************************************/
#ifndef REAGENTPLATE_H_1703563026
#define REAGENTPLATE_H_1703563026

#include "../IDeviceConfig.hpp"
#include "../IDeviceThread.h"
#include "../IModbusProxy.h"
#include "CReagentBin.h"

namespace COM {
class AReagentBin
    : public IDeviceThread
    , public IModbusProxy
    , public IDeviceConfig<CReagentBin> {
    Q_OBJECT
public:
    enum Mode : uint16_t {
        Mode_None      = 0x00,  /// 无(保留模式)
        Mode_Position  = 0x01,  /// 位置模式
        Mode_Velocity  = 0x02,  /// 速度模式
        Mode_BrakeSoft = 0x03,  /// 软刹车
        Mode_BrakeHard = 0x04,  /// 急刹车
        Mode_Homing    = 0x05,  /// 回原点(校准)
        Mode_LED_RGB   = 0x06,  /// LED_RGB
        Mode_RecoveryError,     /// 恢复错误

        Mode_Count  /// 此处有两个作用，1)枚举计数 2)非法标志
    };
    Q_ENUM(Mode)

public:
    explicit AReagentBin(QObject *parent = nullptr);

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
    /**
     * 上升沿操作
     * 速度和位置模式不用上升沿
     */
    RetCode exec_setUpExec();

private:
    RetCode execSet_None();
    RetCode execCheck_None();

    RetCode execSet_Position();
    RetCode execCheck_Position();

    RetCode execSet_Velocity();
    RetCode execCheck_Velocity();

    RetCode execSet_BrakeSoft();
    RetCode execCheck_BrakeSoft();

    RetCode execSet_BrakeHard();
    RetCode execCheck_BrakeHard();

    RetCode execSet_Homing();
    RetCode execCheck_Homing();

    RetCode execSet_LED_RGB();
    RetCode execCheck_LED_RGB();

    RetCode execSet_RecoveryError();
    RetCode execCheck_RecoveryError();
};
}  // namespace COM

#endif  // REAGENTPLATE_H_1703563026

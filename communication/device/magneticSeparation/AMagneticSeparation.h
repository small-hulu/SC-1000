#ifndef TMAGNETICSEPARATION_H_1704794103
#define TMAGNETICSEPARATION_H_1704794103

#include "../IDeviceConfig.hpp"
#include "../IDeviceThread.h"
#include "../IModbusProxy.h"
#include "CMagneticSeparation.h"

namespace COM {

class AMagneticSeparation
    : public IDeviceThread
    , public IModbusProxy
    , public IDeviceConfig<CMagneticSeparation> {
    Q_OBJECT
public:
    enum Mode : uint16_t {
        Mode_None                 = 0x00,  /// 无(保留模式)
        Mode_AbleTrayMove         = 0x01,  /// 托盘运动模式
        Mode_AbleWash             = 0x02,  /// 清洗液加注模式
        Mode_AbleDrainNeedle      = 0x03,  /// 排液针排液模式
        Mode_AbleASubstratePump   = 0x04,  /// 底物A加注模式
        Mode_HomingTray           = 0x05,  /// 托盘回原点(校准)
        Mode_HomingWashPump       = 0x06,  /// 清洗液泵回原点(校准)
        Mode_HomingASubstratePump = 0x07,  /// 底物A泵回原点(校准)
        Mode_HomingZ              = 0x08,  /// z轴homing
        Mode_DebugZ               = 0x09,  /// z轴debug
        Mode_RecoveryError,                /// 恢复错误

        Mode_Count  /// 此处有两个作用，1)枚举计数 2)非法标志
    };
    Q_ENUM(Mode)

public:
    explicit AMagneticSeparation(QObject *parent = nullptr);

public:
    QString to_string() const override;

public:
    void Set_Config(int mode, const Params &params) override {
        m_params    = params;
        m_modeIndex = mode;
    }

public:
    QString   Get_mcuVersion() const override;
    StateBits Get_mcuStateBits() const override;
    Params    Get_mcuParams() const override;
    ErrorPkg  Check_mcuError() const override;

private:
    RetCode set_modeType(Mode mode);
    void    exec_check_modeReady();
    RetCode exec_setUpExec();

private:
    RetCode execSet_None();
    RetCode execCheck_None();

    RetCode execSet_AbleTrayMove();
    RetCode execCheck_AbleTrayMove();

    RetCode execSet_AbleWash();
    RetCode execCheck_AbleWash();

    RetCode execSet_AbleDrainNeedle();
    RetCode execCheck_AbleDrainNeedle();

    RetCode execSet_AbleASubstratePump();
    RetCode execCheck_AbleASubstratePump();

    RetCode execSet_HomingTray();
    RetCode execCheck_HomingTray();

    RetCode execSet_HomingWashPump();
    RetCode execCheck_HomingWashPump();

    RetCode execSet_HomingASubstratePump();
    RetCode execCheck_HomingASubstratePump();

    RetCode execSet_HomingZ();
    RetCode execCheck_HomingZ();

    RetCode execSet_DebugZ();
    RetCode execCheck_DebugZ();

    RetCode execSet_RecoveryError();
    RetCode execCheck_RecoveryError();
};
}  // namespace COM
#endif  // TMAGNETICSEPARATION_H_1704794103

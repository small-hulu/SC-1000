#ifndef TPIPETTINGNEEDLE_H_1709797241
#define TPIPETTINGNEEDLE_H_1709797241

#include "../IDeviceConfig.hpp"
#include "../IDeviceThread.h"
#include "../IModbusProxy.h"
#include "CPipetteNeedle.h"

namespace COM {
/**
 * @brief The WPipettingNeedle class
 * 移液针的 OtherError 大概率是撞针
 */
class APipetteNeedle
    : public IDeviceThread
    , public IModbusProxy
    , public IDeviceConfig<CPipetteNeedle> {
    Q_OBJECT
public:
    enum Mode : uint16_t {
        Mode_None            = 0x00,  /// 无(保留模式)
        Mode_YPosition       = 0x01,  /// y运动模式
        Mode_Suction         = 0x02,  /// 吸液模式
        Mode_Drainage        = 0x03,  /// 排液模式
        Mode_HardBrake       = 0x04,  /// 急刹车
        Mode_Wash            = 0x05,  /// 洗针
        Mode_Homing          = 0x06,  /// 回原点(校准)
        Mode_FillBottle      = 0x07,  /// 补充缓冲液
        Mode_WashOutside     = 0x08,  /// 洗针，只洗外壁
        Mode_MoveSuction     = 0x09,  /// 移动并吸液
        Mode_MoveDrainage    = 0x0A,  /// 移动并排液
        Mode_DebugYZPosition = 0x0B,  /// debug
        Mode_RecoveryError,           /// 恢复错误

        Mode_Count  /// 此处有两个作用，1)枚举计数 2)非法标志
    };
    Q_ENUM(Mode)

public:
    explicit APipetteNeedle(QObject *parent = nullptr);

public:
    QString to_string() const override;

public:
    void Set_Config(int mode, const Params &params) override {
        m_modeIndex = mode;
        m_params    = params;
    }

    QString   Get_mcuVersion() const override;
    StateBits Get_mcuStateBits() const override;
    Params    Get_mcuParams() const override;
    ErrorPkg  Check_mcuError() const override;

private:
    void    exec_check_modeReady();
    RetCode exec_setUpExec();

private:
    RetCode execSet_None();
    RetCode execCheck_None();

    RetCode execSet_YPosition();
    RetCode execCheck_YPosition();

    RetCode execSet_Suction();
    RetCode execCheck_Suction();

    RetCode execSet_Drainage();
    RetCode execCheck_Drainage();

    RetCode execSet_HardBrake();
    RetCode execCheck_HardBrake();

    RetCode execSet_Wash();
    RetCode execCheck_Wash();

    RetCode execSet_Homing();
    RetCode execCheck_Homing();

    RetCode execSet_FillBottle();
    RetCode execCheck_FillBottle();

    RetCode execSet_WashOutside();
    RetCode execCheck_WashOutside();

    RetCode execSet_MoveSuction();
    RetCode execCheck_MoveSuction();

    RetCode execSet_MoveDrainage();
    RetCode execCheck_MoveDrainage();

    RetCode execSet_DebugYZPosition();
    RetCode execCheck_DebugYZPosition();

    RetCode execSet_RecoveryError();
    RetCode execCheck_RecoveryError();
};
}  // namespace COM

#endif  // TPIPETTINGNEEDLE_H_1709797241

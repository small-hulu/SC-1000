#ifndef TMIXING_H_1709793112
#define TMIXING_H_1709793112

#include "../IDeviceConfig.hpp"
#include "../IDeviceThread.h"
#include "../IModbusProxy.h"
#include "CMixing.h"

namespace COM {
class AMixing
    : public IDeviceThread
    , public IModbusProxy
    , public IDeviceConfig<CMixing> {
    Q_OBJECT
public:
    enum Mode : uint16_t {
        Mode_None               = 0x00,  /// 无(保留模式)
        Mode_EnableLoadMaterial = 0x01,  /// 上料使能
        Mode_HomingLoadMaterial = 0x02,  /// 上料校准
        Mode_EnableDivideCup    = 0x03,  /// 分杯使能
        Mode_HomingDivideCup    = 0x04,  /// 分杯校准
        Mode_EnableMix          = 0x05,  /// 混匀使能
        Mode_HomingMix          = 0x06,  /// 混匀校准
        Mode_DebugCup           = 0x07,  /// 推杯Debug
        Mode_RecoveryError,              /// 恢复错误

        Mode_Count  /// 此处有两个作用，1)枚举计数 2)非法标志
    };
    Q_ENUM(Mode)

public:
    explicit AMixing(QObject *parent = nullptr);

public:
    QString to_string() const override;

public:
    void Set_Config(int mode, const Params &params = {}) override {
        m_params    = params;
        m_modeIndex = mode;
    }

    QString   Get_mcuVersion() const override;
    StateBits Get_mcuStateBits() const override;
    Params    Get_mcuParams() const override;
    ErrorPkg  Check_mcuError() const override;

private:
    /// 做一个通用的上升沿操作
    RetCode exec_risingEdge(int startAddress, int bitAddress);

private:
    RetCode execSet_None();
    RetCode execCheck_None();

    RetCode execSet_EnableLoadMaterial();
    RetCode execCheck_EnableLoadMaterial();

    RetCode execSet_HomingLoadMaterial();
    RetCode execCheck_HomingLoadMaterial();

    RetCode execSet_EnableDivideCup();
    RetCode execCheck_EnableDivideCup();

    RetCode execSet_HomingDivideCup();
    RetCode execCheck_HomingDivideCup();

    RetCode execSet_EnableMix();
    RetCode execCheck_EnableMix();

    RetCode execSet_HomingMix();
    RetCode execCheck_HomingMix();

    RetCode execSet_DebugCup();
    RetCode execCheck_DebugCup();

    RetCode execSet_RecoveryError();
    RetCode execCheck_RecoveryError();
};
}  // namespace COM

#endif  // TMIXING_H_1709793112

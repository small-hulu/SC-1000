#ifndef TPHOTOELECTRIC_H_1709795467
#define TPHOTOELECTRIC_H_1709795467

#include "../IDeviceConfig.hpp"
#include "../IDeviceThread.h"
#include "../IModbusProxy.h"
#include "CPhotoelectric.h"

namespace COM {
class APhotoelectric
    : public IDeviceThread
    , public IModbusProxy
    , public IDeviceConfig<CPhotoelectric> {
    Q_OBJECT
public:
    enum Mode : uint16_t {
        Mode_None          = 0x00,  /// 无(保留模式)
        Mode_DoorClose     = 0x01,  /// 检测仓回收
        Mode_DoorOpen      = 0x02,  /// 检测仓打开
        Mode_Detect        = 0x03,  /// 荧光检测
        Mode_HomingPump    = 0x04,  /// 泵归位
        Mode_OnlyDetect    = 0x05,  /// 光电检测
        Mode_OnlyInjection = 0x06,  /// 注液
        Mode_HomingDoor    = 0x07,  /// 舱门归位
        Mode_DebugDoor     = 0x08,  /// debug Door
        Mode_RecoveryError,         /// 恢复错误

        Mode_Count  /// 此处有两个作用，1)枚举计数 2)非法标志
    };
    Q_ENUM(Mode)

public:
    explicit APhotoelectric(QObject *parent = nullptr);

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

public:
    int Get_value();

private:
    /// 做一个通用的上升沿操作
    RetCode exec_risingEdge(int startAddress, int bitAddress);

private:
    RetCode execSet_None();
    RetCode execCheck_None();

    RetCode execSet_DoorClose();
    RetCode execCheck_DoorClose();

    RetCode execSet_DoorOpen();
    RetCode execCheck_DoorOpen();

    RetCode execSet_Detect();
    RetCode execCheck_Detect();

    RetCode execSet_HomingPump();
    RetCode execCheck_HomingPump();

    RetCode execSet_OnlyDetect();
    RetCode execCheck_OnlyDetect();

    RetCode execSet_OnlyInjection();
    RetCode execCheck_OnlyInjection();

    RetCode execSet_HomingDoor();
    RetCode execCheck_HomingDoor();

    RetCode execSet_DebugDoor();
    RetCode execCheck_DebugDoor();

    RetCode execSet_RecoveryError();
    RetCode execCheck_RecoveryError();

signals:
    void signal_DetectValue(qint32);
};
}  // namespace COM
#endif  // TPHOTOELECTRIC_H_1709795467

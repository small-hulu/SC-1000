#ifndef WTHERMOSTAT_H_1710840925
#define WTHERMOSTAT_H_1710840925

#include "../IDeviceConfig.hpp"
#include "../IDeviceThread.h"
#include "../IModbusProxy.h"
#include "CThermostat.h"

namespace COM {
class AThermostat
    : public IDeviceThread
    , public IModbusProxy
    , public IDeviceConfig<CThermostat> {
    Q_OBJECT
public:
    enum Mode : uint16_t {
        Mode_None = 0x00,        /// 无(保留模式)
        Mode_PhotoelectricAble,  /// 光电使能/失能
        Mode_IncubateAble,       /// 温裕盘使能/失能
        Mode_ReagentBinLidAble,  /// 试剂仓盖使能/失能
        Mode_ReagentBinAble,     /// 试剂仓使能/失能
        Mode_Fan0Able,           /// 风扇0使能/失能
        Mode_Fan1Able,           /// 风扇1使能/失能
        Mode_Fan2Able,           /// 风扇2使能/失能
        Mode_Fan3Able,           /// 风扇3使能/失能

        Mode_Count  /// 此处有两个作用，1)枚举计数 2)非法标志
    };
    Q_ENUM(Mode)

public:
    explicit AThermostat(QObject *parent = nullptr);

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
    RetCode set_onceBitState(int startAddress, unsigned bitIdx, int flag);

private:
    RetCode execSet_None();
    RetCode execCheck_None();

    RetCode execSet_PhotoelectricAble();
    RetCode execCheck_PhotoelectricAble();

    RetCode execSet_IncubateAble();
    RetCode execCheck_IncubateAble();

    RetCode execSet_ReagentBinLidAble();
    RetCode execCheck_ReagentBinLidAble();

    RetCode execSet_ReagentBinAble();
    RetCode execCheck_ReagentBinAble();

    RetCode execSet_Fan0Able();
    RetCode execCheck_Fan0Able();

    RetCode execSet_Fan1Able();
    RetCode execCheck_Fan1Able();

    RetCode execSet_Fan2Able();
    RetCode execCheck_Fan2Able();

    RetCode execSet_Fan3Able();
    RetCode execCheck_Fan3Able();
};
}  // namespace COM

#endif  // WTHERMOSTAT_H_1710840925

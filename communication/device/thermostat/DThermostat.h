#ifndef ETHERMOSTAT_H_1710846119
#define ETHERMOSTAT_H_1710846119

#include "../../interface/ISingleton.hpp"
#include "../IDeviceFixture.hpp"
#include "AThermostat.h"

namespace COM {
class DThermostat
    : public AThermostat
    , public IDeviceFixture
    , public ISingleton<DThermostat> {
    Q_OBJECT

public:
    struct {
        double tempPhotoelectric = 37;  /// 光电仓温度
        double tempIncubate      = 37;  /// 温裕盘温度
        double tempReagentBinLid = 37;  /// 试剂仓盖温度
    } fix;

public:
    explicit DThermostat(QObject* parent = nullptr);

public:
    virtual void    Reset_cacheFixtureParams() override;
    virtual void    Save_fileFixtureParams() override;
    virtual RetCode Init_muc() override;

public:
    Params Get_MCUTemp();
};
}  // namespace COM
#endif  // ETHERMOSTAT_H_1710846119

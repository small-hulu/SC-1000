/** ************************************************************************
 * Copyright © 2024 luyinfeng.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DPipettingNeedle.h
 * @date       2024-04-17
 * @version    1.0.0
 *
 * @author     luyinfeng
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 移液针共有6个位置
 * - 温裕盘的上料位
 * - 洗针位置
 * - 试剂仓的四个位置
 *  + 0号位（与1号相连，靠近设备外部）
 *  + 1号位（与0号相连）
 *  + 2号位
 *  + 3号位（靠近设备内部）
 *
 ***************************************************************************/
#ifndef OPIPETTINGNEEDLE_H_1709715270
#define OPIPETTINGNEEDLE_H_1709715270

#include "../../config/SharedMemory.h"
#include "../../interface/ISingleton.hpp"
#include "../../utility/UnitConv.h"
#include "../IDeviceFixture.hpp"
#include "APipetteNeedle.h"

namespace COM {
class DPipetteNeedle
    : public APipetteNeedle
    , public IDeviceFixture
    , public ISingleton<DPipetteNeedle> {
    Q_OBJECT
public:
    struct InfoPkg {
        Params params{};
    };

public:
    using InfoFactory = std::function<InfoPkg()>;
    using InfoTable   = QHash<QString, InfoFactory>;

public:
    struct {
        int             delayWashTime = 3000;
        Params          Wash;
        Params          Load;
        Params          Dilute;
        QVector<Params> ReagentBinList;
    } fix;

    QHash<QString, InfoFactory> m_fixTable;

public:
    explicit DPipetteNeedle(QObject* parent = nullptr);

public:
    void    Reset_cacheFixtureParams() override;
    void    Save_fileFixtureParams() override;
    RetCode Init_muc() override;

public:
    RetCode Set_delayWashTime(int32_t delay);

public:
    InfoPkg Get_fixInfo(const QString& name) const;

public:
    /**
     * 温裕盘的上料位
     * 固定位置-出场生产设置
     * 仅y轴位置
     * 一般来说，该位置只有排液
     */
    InfoPkg GetInfo_to_Load() const;

    /**
     * 温裕盘的稀释位
     * 固定位置-出场生产设置
     * 仅y轴位置
     * 一般来说，该位置只有排液
     */
    InfoPkg GetInfo_to_Dilute() const;

    /**
     * 洗针位
     * 需要设定z轴下探深度
     */
    InfoPkg GetInfo_to_Wash() const;

    /**
     * 到哪个空位，0~6
     * [0,1,2,3] [4,5,6]
     */
    InfoPkg GetInfo_to_ReagentBin(int idx) const;

public:
    /**
     * 对于试剂的随量跟踪模式
     *
     * 针对随量跟踪做处理
     * 1. 由总量和已用量 => 残留量
     * 2. 残留量 => 高度
     * 3. 高度   => 步数
     * ============================
     * 移液针：
     *  1 步 <=> 0.00635 mm
     * ============================
     * 试剂船：
     *         容量(ml)     高度(mm)
     * 0 (A腔)      1  <=>  4.075379855967925
     * 1 (B腔)      1  <=>  3.906793288441674
     * 2 (C腔)      1  <=>  2.80195844566735
     * 3 (D腔)      1  <=>  4.0185568
     */
    void Suction_reagentTrack(Params& params, const int idxBinGroup, const int idxBinBack);
    ;

    /**
     * 对于样本的随量跟踪
     *
     * 一般来说，是对于微量管的
     */
    void Suction_sampleTrack(Params& params, const int idxBinGroup, const int idxBinBack);
};

struct PipetteNeedle_delayWashTime_Guard {
    int32_t oldTime = 0;

    PipetteNeedle_delayWashTime_Guard(int32_t newTime) {
        auto&& device = DPipetteNeedle::instance();
        oldTime       = device.Get_mcuParams().delayWashTime;
        device.Set_delayWashTime(newTime);
    }

    ~PipetteNeedle_delayWashTime_Guard() {
        auto&& device = DPipetteNeedle::instance();
        device.Set_delayWashTime(oldTime);
    }
};

}  // namespace COM

#endif  // OPIPETTINGNEEDLE_H_1709715270

/** ************************************************************************
 * Copyright © 2024 luyinfeng.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DGripper.h
 * @date       2024-04-16
 * @version    1.0.0
 *
 * @author     luyinfeng
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 夹爪部件涉及的其他部件
 * - 上料位
 * - 混匀位
 * - 光电检测
 * - 抛料位
 * - 磁分离
 *  + 位置对应位置
 * - 温裕盘
 *  + 上料位
 *  + 温裕位置
 *
 ***************************************************************************/

/** ************************************************************************
 * ===========================================
 *  <x-----------------------------------------
 * |   (6,0)                               (0,0)
 * |    [6]   [5]   [4]   [3]   [2]   [1]   [0]
 * | [0] o     o     o     o     o     o     o
 * | [1] o     o     o     o     o     o     o
 * | [2] o     o     o     o     o     o     o
 * | [3] o     o     o     o     o     o     o
 * | [4] o     o     o     o     o     o     o
 * | [5] o     o     o     o     o     o     o
 * y [6] o     o     o     o     o     o     o
 * v [7]                         o     o     o
 *                                         (0,7)
 * ===========================================
 ***************************************************************************/

#ifndef OGRIPPER_H_1709717485
#define OGRIPPER_H_1709717485

#include "../../interface/ISingleton.hpp"
#include "../IDeviceFixture.hpp"
#include "AGripper.h"

namespace COM {
class DGripper
    : public AGripper
    , public IDeviceFixture
    , public ISingleton<DGripper> {
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
        int                            rangeMin        = 0;  /// 夹爪收缩最小量
        int                            rangeMax        = 0;  /// 夹爪张开最大量
        int                            zPositionHeight = 0;
        Params                         Cup;
        Params                         Mixing;
        Params                         MagneticSeparation;
        Params                         Photoelectric;
        Params                         Throw;
        Params                         Load;
        Params                         Dilute;
        QHash<int, QHash<int, Params>> Incubate;
        Params                         IncubateA;  /// a(0, 0)
        Params                         IncubateB;  /// b(0, 6)
        Params                         IncubateC;  /// c(6, 0)
    } fix;

private:
    QHash<QString, InfoFactory> m_fixTable;

public:
    explicit DGripper(QObject* parent = nullptr);

public:
    void    Reset_cacheFixtureParams() override;
    void    Save_fileFixtureParams() override;
    void    Save_fileFixtureParams_Without_Z() override;
    RetCode Init_muc() override;

public:
    /**
     * 夹爪张开的最小和最大量
     */
    RetCode Set_rangeDefault(int32_t valClose, int32_t valOpen);

public:
    InfoPkg Get_fixInfo(const QString& name) const;

public:
    //! 获取夹爪到各个位置的位置坐标

    /**
     * 分杯位
     * 固定位置-出场生产设置
     */
    InfoPkg GetInfo_to_Cup() const;

    /**
     * 混匀位
     * 固定位置-出场生产设置
     * TODO
     * 混匀位有4个孔，但是目前只使用一个
     * TODO
     */
    InfoPkg GetInfo_to_Mixing() const;

    /**
     * 光电位
     * 固定位置-出场生产设置
     */
    InfoPkg GetInfo_to_Photoelectric() const;

    /**
     * 抛料位
     * 固定位置-出场生产设置
     */
    InfoPkg GetInfo_to_Throw() const;

    /**
     * 磁分离位
     * 固定位置-出场生产设置
     * +特殊：
     *   需要与磁分离同步的出场设置
     */
    InfoPkg GetInfo_to_MagneticSeparation() const;

    /**
     * 温裕盘的上料位
     * 固定位置-出场生产设置
     */
    InfoPkg GetInfo_to_Load() const;

    /**
     * 温裕盘的稀释位
     * 固定位置-出场生产设置
     */
    InfoPkg GetInfo_to_Dilute() const;

    /**
     * 温裕盘の温裕位置
     * 内部维护状态
     * +特殊：
     *   通过计算或者制表
     *   默认返回空闲位置
     * ===============
     * 这是要计算的，计算出空闲位置
     */
    InfoPkg GetInfo_to_Incubate_byXY(int i, int j) const;

public:
    void Set_ZPositionHeightFlag(bool toTop);
};

struct Gripper_delayWashTime_Guard {
    bool toTop = true;

    Gripper_delayWashTime_Guard(bool toTop = true) : toTop(toTop) {
        auto&& device = DGripper::instance();
        device.Set_ZPositionHeightFlag(toTop);
    }

    ~Gripper_delayWashTime_Guard() {
        auto&& device = DGripper::instance();
        device.Set_ZPositionHeightFlag(!toTop);
    }
};

}  // namespace COM

#endif  // OGRIPPER_H_1709717485

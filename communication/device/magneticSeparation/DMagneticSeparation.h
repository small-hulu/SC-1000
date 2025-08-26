#ifndef OGMAGNETICSEPARATION_H_1709717630
#define OGMAGNETICSEPARATION_H_1709717630

#include "../../interface/ISingleton.hpp"
#include "../IDeviceFixture.hpp"
#include "AMagneticSeparation.h"
#include "CUMagneticSeparation.h"

namespace COM {
class DMagneticSeparation final
    : public AMagneticSeparation
    , public IDeviceFixture
    , public ISingleton<DMagneticSeparation> {
    Q_OBJECT
    friend ISingleton<DMagneticSeparation>;

private:
    using Item = std::function<Params()>;

public:  //! 常量区
    /**
     * 磁分离共有12个孔
     * 但在代码中体现是下标最大到11，即[0, 11)
     */
    static inline const int MCell_Count = MagneticSeparationCount;

public:
    struct FrockFix {
        int offset             = 0;
        int count              = MCell_Count;
        int drainDeep          = 0;
        int mixHeight          = 0;
        int Gripper            = 0;
        int Wash               = 0;
        int Needle0            = 0;
        int Needle1            = 0;
        int Needle2            = 0;
        int ASubstrate         = 0;
        int washCapacity       = 0;
        int aSubstrateCapacity = 0;
    } fix;

private:
    QHash<QString, Item> m_fixTable;
    CUMagneticSeparation m_cu;

private:
    explicit DMagneticSeparation(QObject* parent = nullptr);

public:
    QString to_string() const override;

public:
    /// CU controller-unit 控制器单元
    CUMagneticSeparation& Get_CU() {
        return m_cu;
    }

public:
    void    Reset_cacheFixtureParams() override;
    void    Save_fileFixtureParams() override;
    RetCode Init_muc() override;

public:
    /**
     * 启动 run loop
     * 请保证已经初始化过fun了
     */
    virtual void Start_thread() override {
        IDeviceThread::Start_thread();
        m_cu.Reset();
        m_cu.Start_thread();
    }

    /**
     * 停止 run loop
     */
    virtual void Stop_thread() override {
        m_cu.Stop_thread();
        IDeviceThread::Stop_thread();
    }

public:
    void Calc_byGripper(int pos);

public:
    Params Offset_byProximity(IDeviceConfig::Params& params) const;

    Params Offset_byIndex(Params& params, int idx) const;

    Params Get_fixParams(const QString& name) const;

public:
    /// 对应到和夹爪配合的位置
    Params GetParams_to_Grapper() const {
        Params params{};
        params.isValid            = true;
        params.position           = fix.Gripper;
        params.washCapacity       = fix.washCapacity;
        params.aSubstrateCapacity = fix.aSubstrateCapacity;
        return params;
    }

    /// 到清洗池位置
    Params GetParams_to_Wash() const {
        Params params{};
        params.isValid            = true;
        params.position           = fix.Wash;
        params.washCapacity       = fix.washCapacity;
        params.aSubstrateCapacity = fix.aSubstrateCapacity;
        return params;
    }

    /// 排液针0
    Params GetParams_to_Needle0() const {
        Params params{};
        params.isValid            = true;
        params.position           = fix.Needle0;
        params.washCapacity       = fix.washCapacity;
        params.aSubstrateCapacity = fix.aSubstrateCapacity;
        return params;
    }

    /// 排液针1
    Params GetParams_to_Needle1() const {
        Params params{};
        params.isValid            = true;
        params.position           = fix.Needle1;
        params.washCapacity       = fix.washCapacity;
        params.aSubstrateCapacity = fix.aSubstrateCapacity;
        return params;
    }

    /// 排液针2
    Params GetParams_to_Needle2() const {
        Params params{};
        params.isValid            = true;
        params.position           = fix.Needle2;
        params.washCapacity       = fix.washCapacity;
        params.aSubstrateCapacity = fix.aSubstrateCapacity;
        return params;
    }

    /// 底物A
    Params GetParams_to_ASubstrate() const {
        Params params{};
        params.isValid            = true;
        params.position           = fix.ASubstrate;
        params.washCapacity       = fix.washCapacity;
        params.aSubstrateCapacity = fix.aSubstrateCapacity;
        return params;
    }
};
}  // namespace COM

#endif  // OGMAGNETICSEPARATION_H_1709717630

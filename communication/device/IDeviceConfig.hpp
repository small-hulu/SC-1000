#ifndef IWORKERCONFIG_HPP_707111380
#define IWORKERCONFIG_HPP_707111380

#include <QString>

#include "../config/CLteErrorCode.hpp"

namespace COM {

/**
 * 比较通用的 mcu 信息
 */
struct McuMessage {
    int isValid      = 0;
    int type         = -1;
    int versionMajor = 0;
    int versionMinor = 0;
    int versionPatch = 0;

    bool is_valid() const {
        return isValid;
    }

    QString to_appType() {
        return QString("Type[%1]").arg(type);
    }

    QString to_version() const {
        QString msg = QString("V%1.%2.%3")
                          .arg(versionMajor)  //
                          .arg(versionMinor)  //
                          .arg(versionPatch);
        return msg;
    }

    QString to_string() const {
        QString msg = QString("Type[%1] Version[%2.%3.%4]")
                          .arg(type)          //
                          .arg(versionMajor)  //
                          .arg(versionMinor)  //
                          .arg(versionPatch);
        return msg;
    }
};

/**
 * 参数保证 下面两个using正常
 */
template <typename Config>
struct IDeviceConfig {
public:
    /// 将名称统一到这个类体系中
    /// 注意在设计模板类型的时候也规定这个两个名字
    /// 如果写错，在编译时会生成警告
    using Params    = typename Config::Params;
    using StateBits = typename Config::StateBits;

public:
    using ErrorPkg  = CLteErrorCode::ErrorPkg;
    using ErrorCode = CLteErrorCode::ErrorCode;

protected:
    Params m_params;

public:
    virtual ~IDeviceConfig() = default;

public:
    /**
     * @param mode      枚举的运动状态
     * @param params    参数
     * 注意，在set的时候请先set params
     * 后set mode
     */
    virtual void Set_Config(int mode, const Params& params) = 0;

public:
    /**
     * 获取mcu的信息
     * 如：版本号等等
     */
    virtual QString Get_mcuVersion() const = 0;

    /**
     * 获取mcu的状态表
     * 请注意设计这个状态表的时候的空间效率
     */
    virtual StateBits Get_mcuStateBits() const = 0;

    /**
     * 获取mcu的状态参数
     */
    virtual Params Get_mcuParams() const = 0;

    /**
     * 检查下位机板子是否异常
     */
    virtual ErrorPkg Check_mcuError() const = 0;
};
}  // namespace COM

#endif  // IWORKERCONFIG_HPP_707111380

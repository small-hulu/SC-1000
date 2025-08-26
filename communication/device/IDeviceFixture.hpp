/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       IDeviceFrock.hpp
 * @date       2024-08-10
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 与工装相关的接口
 * 这部分与类型无关
 * 或者说接口，必须是不借助模板的类型擦除技术
 *
 *****************************************************************************/

#ifndef IDEVICEFROCK_HPP_1723258199
#define IDEVICEFROCK_HPP_1723258199

#include <QDebug>
#include <QJsonObject>

#include "../config/CRetCode.h"

namespace COM {
class IDeviceFixture {
public:
    /**
     * file -> cache
     * 文件  -> 缓存
     */
    virtual void Reset_cacheFixtureParams() {
        qCritical() << this << __func__ << "todo";
    }

    /**
     * cache -> file
     * 缓存  ->  文件
     */
    virtual void Save_fileFixtureParams() {
        qCritical() << this << __func__ << "todo";
    }

    /**
     * cache -> file
     * 缓存  ->  文件
     * @brief 保存夹爪配置文件（不包含各点位的z坐标）
     */
    virtual void Save_fileFixtureParams_Without_Z() {
        qCritical() << this << __func__ << "todo";
    }

    /**
     * 给mcu设置默认值
     */
    virtual RetCode Init_muc() {
        qCritical() << this << __func__ << "todo";
        return RetCode::TODO;
    }
};
}  // namespace COM
#endif  // IDEVICEFROCK_HPP_1723258199

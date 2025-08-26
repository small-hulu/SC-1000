/** ************************************************************************
 * Copyright © 2024 luyinfeng.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DIncubate.h
 * @date       2024-04-25
 * @version    1.0.0
 *
 * @author     luyinfeng
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 主要就是状态维护温裕盘的信息，但不控制
 * 温裕盘只有一个温度控制，不在这个模块
 *
 ***************************************************************************/
#ifndef DINCUBATE_H_1714034094
#define DINCUBATE_H_1714034094

#include <QObject>

#include "../../interface/ISingleton.hpp"
#include "../IDeviceFixture.hpp"
#include "CUIncubate.h"

namespace COM {
class DIncubate final
    : public QObject
    , public IDeviceFixture
    , public ISingleton<DIncubate> {
    Q_OBJECT

protected:
    CUIncubate m_cu;

public:
    DIncubate();

public:
    QString to_string() const;

public:
    virtual void    Reset_cacheFixtureParams();
    virtual RetCode Init_muc();

public:
    /// CU controller-unit 控制器单元
    CUIncubate& Get_CU() {
        return m_cu;
    }
};
}  // namespace COM
#endif  // DINCUBATE_H_1714034094

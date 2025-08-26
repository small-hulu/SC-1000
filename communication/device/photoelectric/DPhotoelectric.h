/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       EPhotoelectric.h
 * @date       2024-03-07
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      光电组件模块
 *
 *****************************************************************************/
#ifndef EPHOTOELECTRIC_H_1709796901
#define EPHOTOELECTRIC_H_1709796901

#include "../../interface/ISingleton.hpp"
#include "../IDeviceFixture.hpp"
#include "APhotoelectric.h"
#include "CUPhotoelectric.h"

namespace COM {
class DPhotoelectric
    : public APhotoelectric
    , public IDeviceFixture
    , public ISingleton<DPhotoelectric> {
    Q_OBJECT
public:
    struct {
        int capacity = 0;
        int rangeMin = 0;
    } fix;

private:
    CUPhotoelectric m_cu;

public:
    explicit DPhotoelectric(QObject* parent = nullptr);

public:
    void    Reset_cacheFixtureParams() override;
    void    Save_fileFixtureParams() override;
    RetCode Init_muc() override;

public:
    Params GetParams_defaultCapacity() const {
        Params params{};
        params.isValid  = true;
        params.capacity = fix.capacity;
        return params;
    }

public:
    CUPhotoelectric& Get_CU() {
        return m_cu;
    }
};
}  // namespace COM
#endif  // EPHOTOELECTRIC_H_1709796901

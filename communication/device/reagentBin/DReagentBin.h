/** ************************************************************************
 * Copyright © 2024 luyinfeng.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DReagentBin.h
 * @date       2024-04-20
 * @version    1.0.0
 *
 * @author     luyinfeng
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 *
 *
 ***************************************************************************/
#ifndef OREAGENTBIN_H_1709797196
#define OREAGENTBIN_H_1709797196

#include "../../interface/ISingleton.hpp"
#include "../IDeviceFixture.hpp"
#include "AReagentBin.h"

namespace COM {
class DReagentBin
    : public AReagentBin
    , public IDeviceFixture
    , public ISingleton<DReagentBin> {
    Q_OBJECT

public:
    /// 目前不考虑转速问题，只涉及位置
    struct {
        int          offset = 0;
        int          count  = ReagentBinBinCount;
        QVector<int> backList;
        QVector<int> frontList;
    } fix;

public:
    explicit DReagentBin(QObject* parent = nullptr);

    int Get_periodOfRotation() {
        return fix.offset * fix.count;
    }

public:
    void    Reset_cacheFixtureParams() override;
    void    Save_fileFixtureParams() override;
    RetCode Init_muc() override;

public:
    Params Offset_byProximity(Params& params) const;

    /// 第几个仓
    Params Offset_byIndex(Params& params, int idx) const {
        assert(idx < fix.count);
        params.position += idx * fix.offset;
        return params;
    }

    /// [0, 6]孔到后方
    Params GetInfo_backHole(int idx) {
        assert(idx < fix.backList.size());
        Params params{};
        params.isValid  = true;
        params.position = fix.backList[idx];
        return params;
    }

    /// [0, 6]孔到前方
    Params GetInfo_frontHole(int idx) {
        assert(idx < fix.frontList.size());
        Params params{};
        params.isValid  = true;
        params.position = fix.frontList[idx];
        return params;
    }

public:
    void Set_LED_RGB(int R, int G, int B);

    void Loop_self();
};
}  // namespace COM
#endif  // OREAGENTBIN_H_1709797196

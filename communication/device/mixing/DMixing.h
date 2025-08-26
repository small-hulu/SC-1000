#ifndef OMIXING_H_1709792049
#define OMIXING_H_1709792049

#include "../../interface/ISingleton.hpp"
#include "../IDeviceFixture.hpp"
#include "AMixing.h"
#include "CUMixing.h"

namespace COM {
class DMixing
    : public AMixing
    , public IDeviceFixture
    , public ISingleton<DMixing> {
    Q_OBJECT
public:
    struct FrockFix {
        int rangeMin = 0;
        int rangeMax = 0;
    } fix;

public:
    CUMixing m_cu;

public:
    explicit DMixing(QObject* parent = nullptr);

public:
    void    Reset_cacheFixtureParams() override;
    void    Save_fileFixtureParams() override;
    RetCode Init_muc() override;

public:
    RetCode Init_position(int in, int out);

public:
    CUMixing& Get_CU() {
        return m_cu;
    }

public:
    /**
     * 检测是否分好一个杯
     */
    bool check_cup();
};
}  // namespace COM

#endif  // OMIXING_H_1709792049

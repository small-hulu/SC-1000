#include "ENTITY_ConsumeWasteLiquidTank.h"

#include "communication/COM.hpp"
#include "communication/COM_Device.hpp"

namespace ENTITY {
QString WasteLiquidTank::Get_hintMessage() {
    using namespace COM;
    auto&& device = DMixing::instance();
    if (Helper::can_check(device) == false) {
        return "检测异常";
    }

    /// 废液桶不足
    DMixing::StateBits state = device.Get_mcuStateBits();
    if (state.u1000_Mixing.b08_wasteLiquidTankFull == 1) {
        return "满";
    } else {
        return "不满";
    }
}

double WasteLiquidTank::Get_ratio() {
    using namespace COM;
    auto&& device = DMixing::instance();
    if (Helper::can_check(device) == false) {
        return 0;
    }

    /// 废液桶不足
    DMixing::StateBits state = device.Get_mcuStateBits();
    if (state.u1000_Mixing.b08_wasteLiquidTankFull == 0) {
        return 0;
    } else {
        return 1;
    }
}
}  // namespace ENTITY

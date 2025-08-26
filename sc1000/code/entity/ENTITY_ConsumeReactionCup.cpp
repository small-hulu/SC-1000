#include "ENTITY_ConsumeReactionCup.h"

#include "communication/COM.hpp"
#include "communication/COM_Device.hpp"

namespace ENTITY {

QString ReactionCup::Get_hintMessage() {
    using namespace COM;
    auto&& device = DMixing::instance();
    if (Helper::can_check(device) == false) {
        return "检测异常";
    }

    /// todo
    /// 目前检测的是滑道的标志
    /// 因为直接检测料斗仓失效率太高
    DMixing::StateBits state = device.Get_mcuStateBits();
    if (state.u1000_Mixing.b01_slideFull == 0) {
        return "不足";
    } else {
        return "充足";
    }
}

double ReactionCup::Get_ratio() {
    using namespace COM;
    auto&& device = DMixing::instance();
    if (Helper::can_check(device) == false) {
        return 0;
    }

    DMixing::StateBits state = device.Get_mcuStateBits();
    if (state.u1000_Mixing.b01_slideFull == 0) {
        return 0;
    } else {
        return 1;
    }
}

}  // namespace ENTITY

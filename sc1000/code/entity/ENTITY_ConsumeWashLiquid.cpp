#include "ENTITY_ConsumeWashLiquid.h"

#include "communication/COM.hpp"
#include "communication/COM_Device.hpp"

namespace ENTITY {

QList<QString> WashLiquid::Get_descriptorMessage() {
    const auto info = Get_Info();
    if (info.isValid == false) {
        return {};
    }
    QList<QString> msg;
    msg << QString("高位液量");
    msg << (info.BufferBottleUpper ? QString("充足") : QString("不足"));
    msg << QString("低位液量");
    msg << (info.BufferBottleLower ? QString("充足") : QString("不足"));
    return msg;
}

QString WashLiquid::Get_hintMessage() {
    const auto info = Get_Info();
    if (info.isValid == false) {
        return "检测异常";
    }
    if (info.BufferBottleUpper || info.BufferBottleLower) {
        return "充足";
    }
    return "不足";
}

double WashLiquid::Get_ratio() {
    const auto info = Get_Info();
    if (info.isValid == false) {
        return 0;
    }
    if (info.BufferBottleUpper) {
        return 1;
    }
    if (info.BufferBottleLower) {
        return 0.7;
    }
    return 0;
}

WashLiquid::Info WashLiquid::Get_Info() {
    Info info;

    using namespace COM;
    auto&& device = DPipetteNeedle::instance();
    if (Helper::can_check(device) == false) {
        return info;
    }
    info.isValid = true;

    auto state             = device.Get_mcuStateBits();
    info.BufferBottleUpper = state.u1002.b14_BufferBottleUpper;
    info.BufferBottleLower = state.u1002.b15_BufferBottleLower;
    return info;
}

}  // namespace ENTITY

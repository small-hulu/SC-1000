#include "CDevice_DLightBand.h"

#include "communication/COM_Device.hpp"

namespace CONTROLLER::CDevice {

void DLightBand::Set_LightBandColor(QColor color, const char *const file, const char *const func) {
    auto &&device = COM::DReagentBin::instance();
    if (COM::Helper::can_check(device) == false) {
        qWarning() << "灯带颜色设置失败" << color;
        return;
    }

    qInfo() << QString("From file[%1] func[%2]").arg(file).arg(func)
            << QString("RGB(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    device.Set_LED_RGB(color.red(), color.green(), color.blue());
}

}  // namespace CONTROLLER::CDevice

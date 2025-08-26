#ifndef DLIGHTBAND_H_1727577075
#define DLIGHTBAND_H_1727577075

#include <QColor>

namespace CONTROLLER::CDevice {
struct DLightBand {
public:
    static void Set_LightBandColor(QColor color, const char* const file = __builtin_FILE(),
                                   const char* const func = __builtin_FUNCTION());
};
}  // namespace CONTROLLER::CDevice

#endif  // DLIGHTBAND_H_1727577075

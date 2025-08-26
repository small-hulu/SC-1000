#include "ENTITY_ECalibration.h"

namespace ENTITY {

ENTITY::ECalibration::ECalibration() {
    table[attr::timeStart] = QDateTime::currentDateTime().toString(time_format);
}

/**
 * 根据批号查出瓶号
 */
QString ECalibration::Get_bottleNumber() {
    return "录入错误";
}

}  // namespace ENTITY

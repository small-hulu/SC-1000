#ifndef TYPE_H_1722302339
#define TYPE_H_1722302339

#include <QList>
#include <QString>

#include "global/GLOBAL_TypeTube.h"

namespace Type {

enum class SampleTubeType : int {
    GeneralTube = 0,  /// 普通管
    MicroTube,        /// 微量管
    Other,            /// 其他
};

/**
 * 检测类型的列表
 */
QList<QString> TypeList_of_detect();

/**
 * 试管类型
 */
QList<QString> TypeList_of_tube_type();

/**
 * 年龄的单位
 * 主要照顾小孩的年月日
 */
QList<QString> TypeList_of_ageUnit();

/**
 * 性别的类别
 */
QList<QString> TypeList_of_sex();
};  // namespace Type

#endif  // TYPE_H_1722302339

#ifndef LOG_H_1726137902
#define LOG_H_1726137902

#include "entity/ENTITY_Log.h"

namespace CONTROLLER {
struct Log {
    /// 获取总表格总数
    static int Get_allCount();

    /// 根据不同种类
    static int Get_allCount_byType(int type);

    /// 根据范围获取条目
    static QList<ENTITY::Log> Get_rangeEntity(int start, int end);

    /// 根据具体类型，选择范围
    static QList<ENTITY::Log> Get_rangeEntity_byType(int start, int end, int type);

    /// 清空某个类型
    static bool Clear_byType(int type);
};
}  // namespace CONTROLLER

#endif  // LOG_H_1726137902

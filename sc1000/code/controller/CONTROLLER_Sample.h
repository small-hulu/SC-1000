#ifndef SAMPLE_H_1716961748
#define SAMPLE_H_1716961748

#include "CInterface_DBEntity.hpp"
#include "entity/ENTITY_ESample.h"

namespace CONTROLLER {
class Sample : public CInterface_DBEntity<::ENTITY::ESample> {
public:
    using Self = Sample;
    static Self& instance() {
        static Self obj;
        return obj;
    }

private:
    Sample() = default;

public:
    /**
     * 获取 isLoad 为 1 的列表
     */
    QList<entity_item> Get_isLoadList();

    /**
     * 根据孔位坐标获取装载的内容
     */
    entity_item Get_isLoadByLocation(const QString& location);
};
}  // namespace CONTROLLER

#endif  // SAMPLE_H_1716961748

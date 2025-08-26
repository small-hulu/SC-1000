#ifndef REAGENT_H_1716961597
#define REAGENT_H_1716961597

#include "CInterface_DBEntity.hpp"
#include "entity/ENTITY_EReagent.h"

namespace CONTROLLER {
class Reagent : public CInterface_DBEntity<::ENTITY::EReagent> {
public:
    using Self = Reagent;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

private:
    Reagent();

public:
    QList<entity_item> Select_isLoadList();

    /// 装载一个试剂到设备中
    /// 需要对比历史装载
    bool Load_entity_toCU(entity_item entity);

public:
    /// 根据项目，过滤出集合
    QList<entity_item> FilterList_by_project(const QString& projectName);
    entity_item        Filter_by_batchNum(const QString& batchNum, const QString& project);
};
}  // namespace CONTROLLER

#endif  // REAGENT_H_1716961597

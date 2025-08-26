#include "CONTROLLER_Reagent.h"

namespace CONTROLLER {
Reagent::Reagent() {
    auto get_attrList = [](const QList<entity_item>& items, entity_attr attr) {
        QSet<QString> strList;
        for (auto&& item : items) {
            strList.insert(item->table[attr]);
        }
        return strList;
    };

    auto sub0  = Select_all_fromDB();
    auto items = get_attrList(sub0, entity_attr::project);
    for (QString item : items) {
        auto sub1  = Filter_byAttribute_isEqual(sub0, entity_attr::project, item);
        auto items = get_attrList(sub1, entity_attr::batchNum);
        for (QString item : items) {
            auto attrList = Filter_byAttribute_isEqual(sub1, entity_attr::batchNum, item);
            auto items    = get_attrList(attrList, entity_attr::bottleNum);
            for (QString item : items) {
                auto sub3 = Filter_byAttribute_isEqual(attrList, entity_attr::bottleNum, item);
                for (int i = 0; i < sub3.size() - 1; i += 1) {
                    sub3[i]->Delete_toDB();
                }
            }
        }
    }
}

QList<Reagent::entity_item> Reagent::Select_isLoadList() {
    return Select_byAttribute_isEqual(entity_attr::isLoad, "1");
}

bool Reagent::Load_entity_toCU(CInterface_DBEntity::entity_item entity) {
    auto res = Select_all_fromDB();
    res =
        Filter_byAttribute_isEqual(res, entity_attr::project, entity->table[entity_attr::project]);
    res = Filter_byAttribute_isEqual(res, entity_attr::batchNum,
                                     entity->table[entity_attr::batchNum]);
    res = Filter_byAttribute_isEqual(res, entity_attr::bottleNum,
                                     entity->table[entity_attr::bottleNum]);
    if (res.size()) {
        auto& saved                         = res[0];
        saved->table[entity_attr::isLoad]   = "1";
        saved->table[entity_attr::location] = entity->table[entity_attr::location];
        return saved->Update_toDB();
    } else {
        entity->table[entity_attr::isLoad] = "1";
        return entity->Insert_toDB();
    }
}

QList<Reagent::entity_item> Reagent::FilterList_by_project(const QString& projectName) {
    QList<entity_item> res;
    for (auto&& item : Select_isLoadList()) {
        if (projectName == item->Get_Project()) {
            res << item;
        }
    }
    return res;
}

Reagent::entity_item Reagent::Filter_by_batchNum(const QString& batchNum, const QString& project) {
    for (auto&& item : Select_isLoadList()) {
        if (batchNum == item->table[entity_attr::batchNum] &&
            project == item->table[entity_attr::project]) {
            return item;
        }
    }
    return {};
}

}  // namespace CONTROLLER

#include "CONTROLLER_Sample.h"

namespace CONTROLLER {
QList<Sample::entity_item> Sample::Get_isLoadList() {
    return Select_byAttribute_isEqual(entity_attr::isLoad, "1");
}

Sample::entity_item Sample::Get_isLoadByLocation(const QString& location) {
    auto loadList = Get_isLoadList();
    for (auto&& obj : loadList) {
        if (obj->table[entity_attr::location] == location) {
            return obj;
        }
    }
    return {};
}

}  // namespace CONTROLLER

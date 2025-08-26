#include "ENTITY_ESampleApply.h"

namespace ENTITY {

CONTROLLER::Sample::entity_item ESampleApply::Get_relatedSample() {
    QString isSample = table[attr::idSample];
    return CONTROLLER::Sample::instance().Select_byId_fromDB(isSample);
}

QList<QString> ESampleApply::Get_projectList() {
    QString projectList = table[attr::projectList];
    return projectList.split(join_separator);
}

}  // namespace ENTITY

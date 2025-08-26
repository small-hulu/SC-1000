#ifndef ESAMPLEAPPLY_H_1731401382
#define ESAMPLEAPPLY_H_1731401382

#include "EInterface_DB.hpp"
#include "controller/CONTROLLER_Sample.h"
#include "global/GLOBAL_TypeTube.h"

namespace ENTITY {
class ESampleApply : public EInterface_DB<::DB::TableEntity::sample_apply> {
public:
    ESampleApply() = default;

public:
    /// 获取映射关联的样本
    CONTROLLER::Sample::entity_item Get_relatedSample();
    /// 获取项目列表
    QList<QString> Get_projectList();
};
}  // namespace ENTITY

#endif  // ESAMPLEAPPLY_H_1731401382

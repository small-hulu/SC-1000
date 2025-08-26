#include "DB_TableEntity_sample_apply.h"

#include <QDebug>

namespace DB {
namespace TableEntity {

QString sample_apply::Get_tableName() const {
    return "sample_apply";
}

QString sample_apply::Get_chineseEnum(int idx) const {
    table_attribute e{idx};
    switch (e) {
    case state      : return "状态";
    case idSample   : return "样本id";
    case projectList: return "项目列表";
    case remark     : return "备注";
    case Enum_Count :;
    default         :;
    }
    return "";
}

const QVector<QString>& sample_apply::Get_attributeList() const {
    static QVector<QString> enumList;
    if (enumList.size()) {
        return enumList;
    }
    enumList = QVector<QString>(Enum_Count);

    constexpr int startLine = __LINE__;
    enumList[state]         = "state";
    enumList[idSample]      = "idSample";
    enumList[projectList]   = "projectList";
    enumList[remark]        = "remark";
    constexpr int endLine   = __LINE__;

    return enumList;
    static_assert(Enum_Count == (endLine - startLine - 1));
}

}  // namespace TableEntity
}  // namespace DB

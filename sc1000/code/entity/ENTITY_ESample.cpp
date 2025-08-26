#include "ENTITY_ESample.h"

#include <QDebug>

namespace ENTITY {
void ESample::Set_id(const EInterface_DB::id_type id) {
    this->id = id;
    if (table[attr::number].size() == 0) {
        table[attr::number] = QString::number(id);
    }
}

/**
 * 注意与对应的 append 函数使用的分隔符一致
 */
QList<QString> ESample::Get_projectList() {
    const attr key = attr::project;
    return table[key].split(join_separator);
}

/**
 * 注意与对应的 remove 函数使用的分隔符一致
 */
void ESample::Append_project(const QString &name) {
    const attr key      = attr::project;
    auto       nameList = table[key].split(join_separator);
    if (false == nameList.contains(name)) {
        nameList << name;
        table[key] = nameList.join(join_separator);
    }
}

/**
 * 注意与对应的 append 函数使用的分隔符一致
 */
void ESample::Remove_project(const QString &name) {
    const attr key      = attr::project;
    auto       nameList = table[key].split(join_separator);
    if (true == nameList.contains(name)) {
        nameList.removeOne(name);
        table[key] = nameList.join(join_separator);
    }
}

int ESample::Append_expId(const QString &expid) {
    auto strList = table[attr::expIdList].split(join_separator, QString::SkipEmptyParts);
    if (true == strList.contains(expid)) {
        qWarning() << "重复添加id";
        return -1;
    }
    strList << expid;
    table[attr::expIdList] = strList.join(join_separator);
    /// expValueList 的长度应该与 expIdList 同步
    strList = table[attr::expValueList].split(join_separator, QString::SkipEmptyParts);
    strList << "0";
    table[attr::expValueList] = strList.join(join_separator);
    return strList.size() - 1;
}

QList<QString> ESample::Get_expIdList() {
    return table[attr::expIdList].split(join_separator, QString::SkipEmptyParts);
}

void ESample::Save_expValue_ById(const QString &expid, int value) {
    auto expIdList  = table[attr::expIdList].split(join_separator, QString::SkipEmptyParts);
    auto expValList = table[attr::expValueList].split(join_separator, QString::SkipEmptyParts);
    /// 这两个的长度应该在添加 id 维护的时候就保持一致
    assert(expIdList.size() == expValList.size());

    const int idx = expIdList.indexOf(expid);
    if (idx < 0) {
        qWarning()
            << QString("sample exp 映射错误, sampleId[%1] expId[%2]").arg(this->id).arg(expid);
        return;
    }
    expValList[idx]           = QString::number(value);
    table[attr::expValueList] = expValList.join(join_separator);
    for (auto &&val : expValList) {
        if (val.toInt() == 0) {
            table[attr::state] = QString::number((int)TubeState::Testing);
            return;
        }
    }
    table[attr::state] = QString::number((int)TubeState::Complete);
}
}  // namespace ENTITY

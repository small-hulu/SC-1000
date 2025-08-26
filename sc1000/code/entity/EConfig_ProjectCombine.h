#ifndef PROJECTCOMBINE_H
#define PROJECTCOMBINE_H

#include <QJsonArray>

#include "EInterface_IConfig.h"

namespace ENTITY {
namespace KeyProjectCombine {
const QString ProjectCombine = "ProjectCombine";
const QString name           = "name";
const QString projectList    = "projectList";
}  // namespace KeyProjectCombine

class ProjectCombine : public IConfig {
public:
    struct Combine {
        QString        name;
        QList<QString> projectList;
    };

private:
    QHash<QString, Combine> table;

public:
    ProjectCombine() {
        json           = m_ojson[KeyProjectCombine::ProjectCombine].toObject();
        QJsonArray arr = json[KeyProjectCombine::projectList].toArray();
        for (auto itemVal : arr) {
            QJsonObject item = itemVal.toObject();
            Combine     com;
            com.name        = item[KeyProjectCombine::name].toString();
            QJsonArray list = item[KeyProjectCombine::projectList].toArray();
            for (auto str : list) {
                com.projectList << str.toString();
            }
            table[com.name] = com;
        }
    }
    virtual bool Save() override {
        QJsonArray arr;
        for (auto&& it : table) {
            QJsonObject obj;
            obj[KeyProjectCombine::name] = it.name;
            QJsonArray list;
            for (auto&& s : it.projectList) {
                list << s;
            }
            obj[KeyProjectCombine::projectList] = list;
            arr << obj;
        }
        json[KeyProjectCombine::projectList]       = arr;
        m_ojson[KeyProjectCombine::ProjectCombine] = json;
        return IConfig::Save();
    }

public:
    /**
     * 获取组合名
     */
    QList<QString> Get_CombineNames() {
        QList<QString> res;
        for (auto&& [key, val] : table) {
            res << key;
        }
        return res;
    }

    /**
     * 获取后台存储的所有组合项目
     */
    QList<Combine> Get_CombineList() {
        QList<Combine> res;
        for (auto&& it : table) {
            res << it;
        }
        return res;
    }

    /**
     * 根据名称获取组合内容
     * 如果不在和返回一个空实体
     */
    Combine Get_CombineByName(const QString& name) {
        if (table.count(name)) {
            return table[name];
        }
        return {};
    }

    /**
     * 添加一个组合项目
     * 如果名称相同，则会覆盖
     */
    void Append_Combine(const Combine& combine) {
        table[combine.name] = combine;
        Save();
    }

    /**
     * 根据传入的组合名称删除持久层的数据
     * 如果本身就没有这个名字则什么都不发生
     */
    void Remove_Combine(const QString& name) {
        if (table.count(name)) {
            table.remove(name);
            Save();
        }
    }
};

}  // namespace ENTITY

#endif  // PROJECTCOMBINE_H

/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       CInterface_DBEntity.hpp
 * @date       2024-07-09
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * Controller Unit
 * 简写：CU
 *
 *****************************************************************************/
#ifndef CINTERFACE_DBENTITY_HPP_1720491721
#define CINTERFACE_DBENTITY_HPP_1720491721

#include "entity/EInterface_DB.hpp"

namespace CONTROLLER {

template <typename Entity>
class CInterface_DBEntity {
public:
    using entity_type  = Entity;
    using entity_item  = ::std::shared_ptr<entity_type>;
    using entity_attr  = typename entity_type::attr;
    using entity_idkey = typename entity_type::id_type;

public:
    static entity_item Create_oneEntityItem() {
        return std::make_shared<entity_type>();
    }

public:
    typename entity_type::db_table cdbtable;

public:
    CInterface_DBEntity()          = default;
    virtual ~CInterface_DBEntity() = default;
    Q_DISABLE_COPY_MOVE(CInterface_DBEntity);

public:
    /**
     * 每个实体本身具有 insert/update/delete
     */

    /**
     * 获取 DB 中的所有数据
     */
    QList<entity_item> Select_all_fromDB() {
        auto               dbMatrix = DB::SqlExe::Get_all_value(cdbtable);
        QList<entity_item> itemList;
        std::transform(dbMatrix.begin(), dbMatrix.end(), std::back_inserter(itemList),
                       [](auto&& line) {
                           entity_item entity = Create_oneEntityItem();
                           entity->Set_Values_WithId(line);
                           return entity;
                       });
        return itemList;
    }

    /**
     * 根据 id 获取实体
     * 同时兼容，int 和 string 类型的主键进行查找
     */
    entity_item Select_byId_fromDB(QString id) {
        return Select_byId_fromDB(id.toInt());
    }

    /**
     * 根据 id 获取实体
     */
    entity_item Select_byId_fromDB(entity_idkey id) {
        auto line = DB::SqlExe::Select_oneLine_byId(cdbtable, id);
        if (line.size() == 0) {
            return {};
        }
        entity_item entity = Create_oneEntityItem();
        entity->Set_Values_WithId(line);
        return entity;
    }

    QList<entity_item> Select_byAttribute_isEqual(entity_attr attr, QString val) {
        auto query             = QString(" select * from %1 where %2 = '%3' ");
        query                  = query.arg(cdbtable.Get_tableName());
        query                  = query.arg(cdbtable.Get_attributeList()[(int)attr]);
        query                  = query.arg(val);
        auto               res = DB::SqlExe::Get_ByOutSql(query);
        QList<entity_item> itemList;
        for (int i = 0; i < res.size(); i += 1) {
            entity_item entity = Create_oneEntityItem();
            entity->Set_Values_WithId(res[i]);
            itemList << entity;
        }
        return itemList;
    }

    QList<entity_item> Select_byAttribute_isNotEqual(entity_attr attr, QString val) {
        auto query             = QString(" select * from %1 where %2 != '%3' ");
        query                  = query.arg(cdbtable.Get_tableName());
        query                  = query.arg(cdbtable.Get_attributeList()[(int)attr]);
        query                  = query.arg(val);
        auto               res = DB::SqlExe::Get_ByOutSql(query);
        QList<entity_item> itemList;
        for (int i = 0; i < res.size(); i += 1) {
            entity_item entity = Create_oneEntityItem();
            entity->Set_Values_WithId(res[i]);
            itemList << entity;
        }
        return itemList;
    }

    QList<entity_item> Filter_byAttribute_isEqual(const QList<entity_item>& items, entity_attr attr,
                                                  QString val) {
        QList<entity_item> res;
        for (auto&& item : items) {
            if (item->table[attr] == val) {
                res << item;
            }
        }
        return res;
    }

    QList<QString> Filter_StringList_bybyAttribute(const QList<entity_item>& items,
                                                   entity_attr               attr) {
        QList<QString> strList;
        for (auto&& item : items) {
            strList << item->table[attr];
        }
        return strList;
    }

    QList<QString> Filter_StringListUnique_bybyAttribute(const QList<entity_item>& items,
                                                         entity_attr               attr) {
        QSet<QString> strList;
        for (auto&& item : items) {
            strList << item->table[attr];
        }
        QList<QString> list{strList.begin(), strList.end()};
        qSort(list);
        return list;
    }
};

}  // namespace CONTROLLER
#endif  // CINTERFACE_DBENTITY_HPP_1720491721

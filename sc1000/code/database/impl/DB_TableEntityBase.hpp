/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DB_TableBase.hpp
 * @date       2024-06-06
 * @version    1.0.0
 *
 * @author     陆寅峰
 *
 * @brief
 * 请保证后续子类均为空类
 *
 *****************************************************************************/
#ifndef DB_ENUMBASE_HPP_1688456242
#define DB_ENUMBASE_HPP_1688456242

#include <QVector>

namespace DB {
namespace TableEntity {

/**
 * 表格基类
 */
struct IEntityBase {
    IEntityBase()                   = default;
    IEntityBase(const IEntityBase&) = default;
    virtual ~IEntityBase()          = default;

    virtual QString Get_tableName() const          = 0;
    virtual QString Get_chineseEnum(int idx) const = 0;
};

/**
 * key-value 模式已废弃
 */
struct IEKeyVal : IEntityBase {
    virtual const QVector<QString>& Get_keyList() const   = 0;
    virtual const QVector<QString>& Get_valueList() const = 0;
};

/**
 * attribute 模式近似废弃
 * 可能还有用武之地
 */
struct IEAttribute : IEntityBase {
    virtual const QVector<QString>& Get_attributeList() const = 0;
};

/**
 * 本质和 attribute 一致
 * 主要是为了改为主键自增模式
 */
struct IEAutoIncrement : IEAttribute {
    using TypeAutoIncrement = int;
};

}  // namespace TableEntity
}  // namespace DB

#endif  // DB_ENUMBASE_HPP_1688456242

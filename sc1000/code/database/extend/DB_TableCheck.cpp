/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DB_TableCheck.cpp
 * @date       2024-06-06
 * @version    1.0.0
 *
 * @author     陆寅峰
 *
 * @brief
 * 最初的设计中，有一个key-value模式的数据库
 * 也就是两列:
 * + 一列key
 * + 一列value
 * 但这钟简单的模式，数据库是大材小用了，直接改用ini文件
 * 把业务删了，框架代码先保留
 * =============================================
 * 另一方面，原先裸的 Attribute 模式的表格改为主键自增模式
 * 依赖主键自增的方式，能够更加有效的进行数据的管理和内部逻辑的操作
 *****************************************************************************/
#include "DB_TableCheck.h"

#include <QDateTime>
#include <QDebug>

#include "../impl/DB_TableFactory_AttributeCol.h"
#include "../impl/DB_TableFactory_Autoincrement.h"
#include "../impl/DB_TableFactory_KeyValue.h"
#include "DB_SqlExe.h"
#include "DB_TableEntity_calibration.h"
#include "DB_TableEntity_consume.h"
#include "DB_TableEntity_expcalibration.h"
#include "DB_TableEntity_expquality.h"
#include "DB_TableEntity_expsample.h"
#include "DB_TableEntity_log.h"
#include "DB_TableEntity_quality.h"
#include "DB_TableEntity_reagent.h"
#include "DB_TableEntity_sample.h"
#include "DB_TableEntity_sample_apply.h"
#include "DB_TableEntity_user.h"

namespace DB {

/**
 * 检查所有表
 * 含首次创建
 */
bool TableCheck::Check_allTable() {
    bool flag = true;

    TableEntity::user user;
    Autoincrement     table(user.Get_tableName(), user.Get_attributeList());
    flag &= table.Check_table();

    TableEntity::reagent reagent;
    table  = Autoincrement(reagent.Get_tableName(), reagent.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::sample sample;
    table  = Autoincrement(sample.Get_tableName(), sample.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::sample_apply sample_apply;
    table  = Autoincrement(sample_apply.Get_tableName(), sample_apply.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::expsample exp;
    table  = Autoincrement(exp.Get_tableName(), exp.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::quality qua;
    table  = Autoincrement(qua.Get_tableName(), qua.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::calibration cal;
    table  = Autoincrement(cal.Get_tableName(), cal.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::expcalibration expcal;
    table  = Autoincrement(expcal.Get_tableName(), expcal.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::expquality expquality;
    table  = Autoincrement(expquality.Get_tableName(), expquality.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::log log;
    table  = Autoincrement(log.Get_tableName(), log.Get_attributeList());
    flag  &= table.Check_table();

    TableEntity::consume consume;
    table  = Autoincrement(consume.Get_tableName(), consume.Get_attributeList());
    flag  &= table.Check_table();

    return flag;
}
}  // namespace DB

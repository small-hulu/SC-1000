/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DB_TableEntity_log.h
 * @date       2024年9月3日
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * ！！！
 * 这个模块的日志不是单独的记录代码逻辑
 * 而是在报警界面的显示，所对应的
 * ！！！
 *
 *****************************************************************************/
#ifndef LOG_H_1719892048
#define LOG_H_1719892048

#include "../impl/DB_TableEntityBase.hpp"

namespace DB {
namespace TableEntity {
struct log : IEAutoIncrement {
    enum table_attribute : int {
        message,  // 信息
        type,     // 日志类型
        time,     // 时间
        file,     // 文件
        func,     // 函数名
        line,     // 行号
        remark,   // 备注

        Enum_Count
    };

    QString                 Get_tableName() const override;
    QString                 Get_chineseEnum(int idx) const override;
    const QVector<QString>& Get_attributeList() const override;
};
}  // namespace TableEntity
}  // namespace DB

#endif  // LOG_H_1719892048

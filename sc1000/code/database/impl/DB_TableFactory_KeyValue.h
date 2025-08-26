#ifndef KEYVALUE_H_1688442680
#define KEYVALUE_H_1688442680

#include <QVector>

#include "DB_TableFactory.h"

namespace DB {

class KeyValue final : public TableFactory {
private:
    QVector<QString> m_keyList;
    QVector<QString> m_valueList;

public:
    KeyValue(const QString& tableName, const QVector<QString>& keyList,
             const QVector<QString>& valueList);

public:
    bool Check_table() override;

private:
    bool create_table() override;
    bool check_tableStruct() override;

private:
    /// check table
    /// 初始化表的key-value
    bool Set_initTable();
    /// 清空原来的数据
    bool delete_table();
    /// 插入初始化的数据
    bool insert_one(const QString& key, const QString& value);
};

}  // namespace DB

#endif  // KEYVALUE_H_1688442680

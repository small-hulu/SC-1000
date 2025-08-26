#ifndef SQLEXE_H_1688522893
#define SQLEXE_H_1688522893

#include <QSqlQuery>

#include "DB_TableEntity_sample.h"

namespace DB {

struct SqlExe {
public:
    static QSqlQuery get_sqlQueryObj();

public:  // all table
    /// 通过外部传入sql执行，完全依赖于外部自己
    static QList<QList<QString>> Get_ByOutSql(const QString& query);
    /// select 所有内容
    static QList<QList<QString>> Get_all_value(const DB::TableEntity::IEntityBase& table);
    /// select 闭区间[start, end]的内容
    static QList<QList<QString>> Get_between(const DB::TableEntity::IEntityBase& table,
                                             const int start, const int end);
    /// 查询总用有多少项
    static int Get_all_count(const DB::TableEntity::IEntityBase& table);

public:  // key-value
    /// kv表 根据key获取value
    static QString Get_keyValue(const ::DB::TableEntity::IEKeyVal& table, int key);
    /// kv表 根据key设置value
    static bool Set_keyValue(const ::DB::TableEntity::IEKeyVal& table, int key,
                             const QString& value);

public:  // attribute
    /// 插入一行
    static bool Insert_oneLine(const TableEntity::IEAttribute& table,
                               const QVector<QString>&         lineMsg);
    /// 根据name删除一行
    static bool Delete_oneLine_byId(const TableEntity::IEAttribute& table, const QString& name);
    static QVector<QString> Select_oneLine_byName(const TableEntity::IEAttribute& table,
                                                  const QString&                  name);
    /// 是否用拥有name行
    static bool Has_oneLine_byName(const TableEntity::IEAttribute& table, const QString& name);
    /// 跟新name行
    static bool Update_oneLine_byId(const TableEntity::IEAttribute& table,
                                    const QList<QString>&           lineMsg);

public:
    /**
     * 返回最新插入元素的id
     * 若为0，表示insert-error
     */
    static int Insert_oneLine(const TableEntity::IEAutoIncrement& table,
                              const QList<QString>&               lineMsg);
    /**
     * 根据id筛选出一项
     * 如果是不存在的id，则返回空序列
     */
    static QList<QString> Select_oneLine_byId(const TableEntity::IEAutoIncrement& table, int id);

    /**
     * 无脑更新所有的项
     * 如果是一个id不存在的，也会正常执行sql，只是什么效果都没有罢了
     */
    static bool Update_oneLine_byId(const TableEntity::IEAutoIncrement& table,
                                    const QList<QString>& lineMsg, int id);

    /**
     * 根据id删除一个信息
     */
    static bool Delete_oneLine_byId(const TableEntity::IEAutoIncrement& table, int id);

    /**
     * 删除所有内容
     */
    static bool Delete_allLines(const TableEntity::IEntityBase& table);
};

}  // namespace DB

#endif  // SQLEXE_H_1688522893

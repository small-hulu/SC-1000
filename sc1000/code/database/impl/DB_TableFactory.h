#ifndef TABLECONSTRUCT_H_1688443291
#define TABLECONSTRUCT_H_1688443291

#include <QSqlDatabase>
#include <QString>

namespace DB {

class TableFactory {
protected:
    QString      m_tableName;
    QSqlDatabase m_dbConnect;

public:
    TableFactory(const QString& tableName);
    virtual ~TableFactory() = default;

public:
    virtual bool Check_table() = 0;

protected:
    /// 是否存在该表，逻辑一致
    bool isExist_table();

protected:
    /// 根据不同类型的表而定
    virtual bool check_tableStruct() = 0;
    virtual bool create_table()      = 0;
};

}  // namespace DB

#endif  // TABLECONSTRUCT_H_1688443291

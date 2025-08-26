#ifndef ATTRIBUTECOL_H_1688453382
#define ATTRIBUTECOL_H_1688453382

#include <QVector>

#include "DB_TableFactory.h"

namespace DB {

class AttributeCol : public TableFactory {
protected:
    QVector<QString> m_attributeList;

public:
    AttributeCol(const QString& tableName, const QVector<QString>& attributeList);

public:
    bool Check_table() override;

protected:
    bool create_table() override;
    bool check_tableStruct() override;

private:
    bool drop_table();
};

}  // namespace DB

#endif  // ATTRIBUTECOL_H_1688453382

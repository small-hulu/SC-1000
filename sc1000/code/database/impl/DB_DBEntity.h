#ifndef DBCONTROLLER_H_1688455598
#define DBCONTROLLER_H_1688455598

#include <QSqlDatabase>
#include <QString>

namespace DB {

class DBEntity {
public:
    static bool Open(const QString& DBFilePath, const QString& connectName);
    static bool Close();

public:
    static const QSqlDatabase& Get_connection();

private:
    static QSqlDatabase global_systemDB_Entity;
};

}  // namespace DB

#endif  // DBCONTROLLER_H_1688455598

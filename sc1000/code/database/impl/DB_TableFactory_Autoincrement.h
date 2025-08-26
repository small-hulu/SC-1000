#ifndef AUTOINCREMENT_H_1717645543
#define AUTOINCREMENT_H_1717645543

#include "DB_TableFactory_AttributeCol.h"

namespace DB {

class Autoincrement : public AttributeCol {
public:
    using AttributeCol::AttributeCol;

protected:
    bool create_table() override;
    bool check_tableStruct() override;
};
}  // namespace DB

#endif  // AUTOINCREMENT_H_1717645543

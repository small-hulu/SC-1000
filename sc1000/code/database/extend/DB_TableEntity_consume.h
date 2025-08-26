#ifndef DB_KEYVALUE_CONSUME_H_1725347935
#define DB_KEYVALUE_CONSUME_H_1725347935

#include "../impl/DB_TableEntityBase.hpp"

namespace DB::TableEntity {
struct consume : IEAutoIncrement {
    enum table_attribute : int {
        type,     /// 类型
        qrcode,   /// 条码
        usedVal,  /// 使用量
        time,     /// 时间
        remark,   /// 备注

        Enum_Count
    };

    virtual QString                 Get_tableName() const;
    virtual QString                 Get_chineseEnum(int idx) const;
    virtual const QVector<QString>& Get_attributeList() const;
};

}  // namespace DB::TableEntity

#endif  // DB_KEYVALUE_CONSUME_H_1725347935

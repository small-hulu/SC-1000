#ifndef EQUALITY_H_1719971534
#define EQUALITY_H_1719971534

#include "EInterface_DB.hpp"
#include "qrcode/QRCODE.hpp"

namespace ENTITY {
class EQuality : public EInterface_DB<::DB::TableEntity::quality> {
public:
    EQuality() = default;

public:
    virtual std::size_t Get_hash() override {
        return UTILITY::get_hash(id, qHash(table[attr::qrcode]));
    }

    QRCODE::EQuality Get_qrcodeEntity() {
        QRCODE::EQuality qr(table[attr::qrcode].toUtf8());
        return qr;
    }

public:
    QList<QString> Get_projectList();

    QDateTime Get_expirationDate();
};
}  // namespace ENTITY

#endif  // EQUALITY_H_1719971534

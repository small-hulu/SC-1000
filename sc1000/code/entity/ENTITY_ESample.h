#ifndef ESAMPLE_H_1716274844
#define ESAMPLE_H_1716274844

#include "EInterface_DB.hpp"
#include "global/GLOBAL_TypeTube.h"

namespace ENTITY {
class ESample : public EInterface_DB<::DB::TableEntity::sample> {
public:
    using TubeState = Type::ETubeState;

public:
    ESample() = default;

    virtual std::size_t Get_hash() override {
        auto hash = UTILITY::get_hash(id, qHash(table[attr::qrcode]));
        return UTILITY::get_hash(hash, reinterpret_cast<std::size_t>(this));
    }

public:
    virtual void Set_id(const id_type id) override;

public:
    QList<QString> Get_projectList();
    void           Append_project(const QString& name);
    void           Remove_project(const QString& name);
    int            Append_expId(const QString& expid);
    QList<QString> Get_expIdList();
    void           Save_expValue_ById(const QString& expid, int value);
};
}  // namespace ENTITY
#endif  // ESAMPLE_H_1716274844

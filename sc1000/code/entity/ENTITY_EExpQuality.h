#ifndef EEXPQUALITY_H_1721286859
#define EEXPQUALITY_H_1721286859

#include "EInterface_DB.hpp"
#include "controller/CONTROLLER_Quality.h"
#include "controller/CONTROLLER_Reagent.h"
#include "global/GLOBAL_TypeTube.h"

namespace ENTITY {
class EExpQuality : public EInterface_DB<::DB::TableEntity::expquality> {
public:
    using ETubeState = Type::ETubeState;

public:
    EExpQuality();

public:
    CONTROLLER::Reagent::entity_item Get_relatedReagent();
    CONTROLLER::Quality::entity_item Get_relatedQuality();

public:
    void Set_expData(int data);

public:
    virtual std::size_t Get_hash() override {
        return UTILITY::get_hash(id, reinterpret_cast<std::size_t>(this));
    }

    double Get_Concentration();

    bool Is_qualified();
};
}  // namespace ENTITY
#endif  // EEXPQUALITY_H

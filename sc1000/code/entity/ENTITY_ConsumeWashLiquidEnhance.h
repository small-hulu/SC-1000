#ifndef WASHLIQUIDENHANCE_H_1726735986
#define WASHLIQUIDENHANCE_H_1726735986

#include "EInterface_Consume.hpp"

namespace ENTITY {
class WashLiquidEnhance : public EInterface_Consume {
public:
    WashLiquidEnhance() = default;

public:
    QString Get_consumeName() const override {
        return "强化清洗液";
    }

public:
    virtual QString Get_hintMessage() override;
    virtual double  Get_ratio() override;
};
}  // namespace ENTITY

#endif  // WASHLIQUIDENHANCE_H_1726735986

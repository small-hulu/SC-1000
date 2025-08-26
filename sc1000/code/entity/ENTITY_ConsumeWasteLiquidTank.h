#ifndef WASTELIQUIDTANK_H_1726732855
#define WASTELIQUIDTANK_H_1726732855

#include "EInterface_Consume.hpp"

namespace ENTITY {
class WasteLiquidTank : public EInterface_Consume {
public:
    WasteLiquidTank() = default;

public:
    QString Get_consumeName() const override {
        return "废液桶";
    }

public:
    virtual QString Get_hintMessage() override;
    virtual double  Get_ratio() override;
};
}  // namespace ENTITY

#endif  // WASTELIQUIDTANK_H_1726732855

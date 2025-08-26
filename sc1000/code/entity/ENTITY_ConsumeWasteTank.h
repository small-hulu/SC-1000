#ifndef WASTETANK_H_1726734417
#define WASTETANK_H_1726734417

#include "EInterface_Consume.hpp"

namespace ENTITY {
class WasteTank : public EInterface_Consume {
private:
    /// 废料箱所能存储的容量
    static constexpr int MaxCup = 200;

public:
    WasteTank() = default;

public:
    QString Get_consumeName() const override {
        return "废料箱";
    }

public:
    virtual QString Get_hintMessage() override;
    virtual double  Get_ratio() override;
};
}  // namespace ENTITY

#endif  // WASTETANK_H_1726734417

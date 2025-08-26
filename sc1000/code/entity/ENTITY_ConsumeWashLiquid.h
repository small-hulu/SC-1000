#ifndef WASHLIQUID_H_1726735663
#define WASHLIQUID_H_1726735663

#include "EInterface_Consume.hpp"

namespace ENTITY {
class WashLiquid : public EInterface_Consume {
public:
    struct Info {
        bool isValid           = false;
        int  BufferBottleUpper = 0;
        int  BufferBottleLower = 0;
    };

public:
    WashLiquid() = default;

public:
    QString Get_consumeName() const override {
        return "清洗液";
    }

public:
    virtual QList<QString> Get_descriptorMessage() override;
    virtual QString        Get_hintMessage() override;
    virtual double         Get_ratio() override;

private:
    Info Get_Info();
};
}  // namespace ENTITY

#endif  // WASHLIQUID_H_1726735663

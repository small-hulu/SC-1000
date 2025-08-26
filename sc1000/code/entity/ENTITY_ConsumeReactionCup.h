#ifndef REACTIONCUP_H_1726734852
#define REACTIONCUP_H_1726734852

#include "EInterface_Consume.hpp"

namespace ENTITY {
class ReactionCup : public EInterface_Consume {
public:
    ReactionCup() = default;

public:
    QString Get_consumeName() const override {
        return "反应杯";
    }

public:
    virtual QString Get_hintMessage() override;
    virtual double  Get_ratio() override;
};
}  // namespace ENTITY

#endif  // REACTIONCUP_H_1726734852

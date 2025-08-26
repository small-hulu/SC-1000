#ifndef PHOTOELECTRICBCAPACITY_H_1723530150
#define PHOTOELECTRICBCAPACITY_H_1723530150

#include "../WOperationPanel.h"
#include "Photoelectric.h"

namespace COM::WFIXTURE {
class PhotoelectricBCapacity : public Photoelectric {
public:
    PhotoelectricBCapacity();

public:
    bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE

#endif  // PHOTOELECTRICBCAPACITY_H_1723530150

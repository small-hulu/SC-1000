#ifndef PHOTOELECTRICRANGEMIN_H_1725267773
#define PHOTOELECTRICRANGEMIN_H_1725267773

#include "Photoelectric.h"

namespace COM::WFIXTURE {
class PhotoelectricRangeMin : public Photoelectric {
public:
    PhotoelectricRangeMin();

public:
    virtual void exec_fixtureMove() override;

public:
    bool Save_fixtureParams() override;
};
}  // namespace COM::WFIXTURE

#endif  // PHOTOELECTRICRANGEMIN_H_1725267773

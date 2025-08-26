#ifndef FSWITCHPAGESTART_H_1697013048
#define FSWITCHPAGESTART_H_1697013048

#include "WIDGET_IWidgetFactory.hpp"

namespace WIDGET {

class FSwitchPageHomepage final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET
#endif  // FSWITCHPAGESTART_H_1697013048

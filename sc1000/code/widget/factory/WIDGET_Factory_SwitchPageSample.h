#ifndef FSWITCHPAGESAMPLE_H_1697014572
#define FSWITCHPAGESAMPLE_H_1697014572

#include "WIDGET_IWidgetFactory.hpp"

namespace WIDGET {
class FSwitchPageSample final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET

#endif  // FSWITCHPAGESAMPLE_H_1697014572

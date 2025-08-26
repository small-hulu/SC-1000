#ifndef FSWITCHPAGEREAGENT_H_1715571879
#define FSWITCHPAGEREAGENT_H_1715571879

#include "WIDGET_IWidgetFactory.hpp"

namespace WIDGET {
class FSwitchPageReagent final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET

#endif  // FSWITCHPAGEREAGENT_H_1715571879

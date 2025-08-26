#ifndef FSWITCHPAGEHELP_H_1724204993
#define FSWITCHPAGEHELP_H_1724204993

#include "WIDGET_IWidgetFactory.hpp"

namespace WIDGET {
class FSwitchPageHelp final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET

#endif  // FSWITCHPAGEHELP_H

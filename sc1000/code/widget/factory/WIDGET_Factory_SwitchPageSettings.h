#ifndef FSWITCHPAGESETTINGS_H_1717143917
#define FSWITCHPAGESETTINGS_H_1717143917

#include "WIDGET_IWidgetFactory.hpp"

namespace WIDGET {
class FSwitchPageSettings final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};

}  // namespace WIDGET
#endif  // FSWITCHPAGESETTINGS_H

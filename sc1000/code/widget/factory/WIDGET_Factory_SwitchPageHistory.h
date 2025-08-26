#ifndef FSWITCHPAGEHISTORY_H_1724203368
#define FSWITCHPAGEHISTORY_H_1724203368

#include "WIDGET_IWidgetFactory.hpp"

namespace WIDGET {
class FSwitchPageHistory final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET
#endif  // FSWITCHPAGEHISTORY_H_1724203368

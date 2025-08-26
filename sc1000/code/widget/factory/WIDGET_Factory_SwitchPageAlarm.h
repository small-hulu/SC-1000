#ifndef ALARM_H_1720067713
#define ALARM_H_1720067713

#include "WIDGET_IWidgetFactory.hpp"

namespace WIDGET {

class FSwitchPageAlarm final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET
#endif  // ALARM_H_1720067713

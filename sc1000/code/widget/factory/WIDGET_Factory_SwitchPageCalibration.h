#ifndef FSWITCHPAGECALIBRATION_H_1719970734
#define FSWITCHPAGECALIBRATION_H_1719970734

#include "WIDGET_IWidgetFactory.hpp"
namespace WIDGET {
class FSwitchPageCalibration final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET

#endif  // FSWITCHPAGECALIBRATION_H_1719970734

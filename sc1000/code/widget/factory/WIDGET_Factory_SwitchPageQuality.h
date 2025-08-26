#ifndef FSWITCHPAGEQUALITY_H_1719970655
#define FSWITCHPAGEQUALITY_H_1719970655

#include "WIDGET_IWidgetFactory.hpp"
namespace WIDGET {
class FSwitchPageQuality final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET

#endif  // FSWITCHPAGEQUALITY_H_1719970655

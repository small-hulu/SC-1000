#ifndef FSWITCHPAGEMAINTAIN_H_1717403934
#define FSWITCHPAGEMAINTAIN_H_1717403934

#include "WIDGET_IWidgetFactory.hpp"
namespace WIDGET {
class FSwitchPageMaintain final : public ISwitchPageFactory {
public:
    using ISwitchPageFactory::ISwitchPageFactory;

    virtual QVector<QString> Get_subNameList() override;
    virtual QWidget         *Create_bySwitchEnum(int index, QWidget *parent) override;
};
}  // namespace WIDGET
#endif  // FSWITCHPAGEMAINTAIN_H

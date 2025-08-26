#ifndef FLEVEL1_H_1696834795
#define FLEVEL1_H_1696834795

#include "WIDGET_IWidgetFactory.hpp"

namespace WIDGET {
class FSwitchPageMain final : public ISwitchPageFactory {
public:
    static QVector<QString> Get_subPageList(int index);

public:
    virtual QVector<QString> Get_subNameList() override {
        return Get_subPageList(-1);
    }
    virtual QWidget* Create_bySwitchEnum(int index, QWidget* parent = nullptr) override;
};
}  // namespace WIDGET

#endif  // FLEVEL1_H_1696834795

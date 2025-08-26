#ifndef WIDGET_IFACTORY_HPP_1696750791
#define WIDGET_IFACTORY_HPP_1696750791

#include <QVector>

class QWidget;

namespace WIDGET {
/**
 * @brief The INameWidgetFactory class
 * ui list工厂接口
 * 返回ui和name
 */
class INameWidgetList {
public:
    virtual ~INameWidgetList() = default;

public:
    /// list for widgetName & widgetPtr
    virtual QVector<QPair<QString, QWidget*>> Create_nameWidgetList(void) = 0;
};

/**
 * @brief The IWidgetFactory class
 * ui list工厂接口
 * 返回ui
 */
class IWidgetList {
public:
    virtual ~IWidgetList() = default;

public:
    /// list for widgetPtr
    virtual QVector<QWidget*> Create_widgetList(void) = 0;
};

/**
 * @brief The IWidgeteSwitchPageFactory class
 * 同级界面切换工厂接口
 */
class ISwitchPageFactory {
public:
    using unique_type = int;

private:
    unique_type identifyName;

public:
    ISwitchPageFactory(const unique_type& name = {}) : identifyName(name) {}

    virtual ~ISwitchPageFactory() = default;

public:
    unique_type Get_identifyName() {
        return identifyName;
    }

public:
    virtual QVector<QString> Get_subNameList()                               = 0;
    virtual QWidget*         Create_bySwitchEnum(int index, QWidget* parent) = 0;
};

}  // namespace WIDGET

#endif  // WIDGET_IFACTORY_HPP_1696750791

#ifndef LIFECYCLE_H_1720237438
#define LIFECYCLE_H_1720237438

#include <QStyle>
#include <QWidget>

namespace WIDGET::UTILS {

template <typename WidgetUi,
          typename = typename std::enable_if_t<std::is_base_of_v<QWidget, WidgetUi>>>
void destroy_widget_refPointer(WidgetUi*& widget) {
    if (widget) {
        widget->close();
        widget = nullptr;
    }
}

/**
 * 一般用于qss中设置属性样式
 * 并在cpp代码中改变了属性后调用
 */
inline void style_polish(QWidget* widget) {
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
}
}  // namespace WIDGET::UTILS

#endif  // LIFECYCLE_H_1720237438

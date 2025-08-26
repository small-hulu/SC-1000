#ifndef BACKGROUND_H_1715828931
#define BACKGROUND_H_1715828931

#include <QWidget>

namespace WIDGET {
class Background {
private:
    static QWidget *view_background;

public:
    static inline void Set(QWidget *background) {
        assert(nullptr == view_background);
        view_background = background;
    }

    static inline QWidget *Get() {
        assert(view_background);
        return view_background;
    }

    static inline QWidget *Exchange(QWidget *&widget) {
        if (nullptr == widget) {
            widget = Get();
        }
        return widget;
    }
};
}  // namespace WIDGET

#endif  // BACKGROUND_H_1715828931

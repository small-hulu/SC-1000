#ifndef WIDGET_START_HOMEPAGE_H_1697013871
#define WIDGET_START_HOMEPAGE_H_1697013871

#include <QWidget>

namespace Ui {
class Homepage;
}

namespace WIDGET::HOMEPAGE {
class Homepage final : public QWidget {
    Q_OBJECT
private:
    Ui::Homepage *ui = nullptr;

public:
    explicit Homepage(QWidget *parent = nullptr);
    virtual ~Homepage();
};
}  // namespace WIDGET::HOMEPAGE

#endif  // WIDGET_START_HOMEPAGE_H_1697013871

#ifndef WHOMEPAGE_DEVICESTATE_H_1715416710
#define WHOMEPAGE_DEVICESTATE_H_1715416710

#include <QWidget>

namespace Ui {
class DeviceState;
}

namespace WIDGET {
namespace HOMEPAGE {
class DeviceState : public QWidget {
    Q_OBJECT
private:
    Ui::DeviceState *ui;

public:
    explicit DeviceState(QWidget *parent = nullptr);
    ~DeviceState();

protected:
    void showEvent(QShowEvent *event) override;
};
}  // namespace HOMEPAGE
}  // namespace WIDGET

#endif  // WHOMEPAGE_DEVICESTATE_H_1715416710

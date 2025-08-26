#ifndef WIDGET_HELPABOUTDEVICE_H_1724225288
#define WIDGET_HELPABOUTDEVICE_H_1724225288

#include <QWidget>

namespace Ui {
class AboutDevice;
}

namespace WIDGET::HELP {
class AboutDevice : public QWidget {
    Q_OBJECT
private:
    Ui::AboutDevice* ui;

public:
    explicit AboutDevice(QWidget* parent = nullptr);
    ~AboutDevice();
};
}  // namespace WIDGET::HELP

#endif  // WIDGET_HELPABOUTDEVICE_H_1724225288

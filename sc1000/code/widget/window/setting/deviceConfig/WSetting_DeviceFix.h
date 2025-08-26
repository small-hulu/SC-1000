#ifndef WSETTING_DEVICEFIX_H_1726154585
#define WSETTING_DEVICEFIX_H_1726154585

#include <QWidget>

namespace Ui {
class DeviceFix;
}

namespace WIDGET::SETTING {
class DeviceFix : public QWidget {
    Q_OBJECT
private:
    Ui::DeviceFix *ui = nullptr;

public:
    explicit DeviceFix(QWidget *parent = nullptr);
    ~DeviceFix();
};
}  // namespace WIDGET::SETTING

#endif  // WSETTING_DEVICEFIX_H_1726154585

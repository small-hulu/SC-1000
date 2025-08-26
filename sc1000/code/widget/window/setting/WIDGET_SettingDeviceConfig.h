#ifndef WIDGET_SETTINGDEVICE_H_1723700644
#define WIDGET_SETTINGDEVICE_H_1723700644

#include <QListWidget>
#include <QWidget>

namespace Ui {
class DeviceConfig;
}

namespace WIDGET::SETTING {
class DeviceConfig : public QWidget {
    Q_OBJECT
private:
    Ui::DeviceConfig* ui = nullptr;

public:
    explicit DeviceConfig(QWidget* parent = nullptr);
    ~DeviceConfig();

private:
    void build_optionList(QListWidget* list, QVector<QString> btnList);
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_SETTINGDEVICE_H_1723700644

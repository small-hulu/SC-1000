#ifndef WIDGET_SETTINGUSERMANAGE_H_1724232985
#define WIDGET_SETTINGUSERMANAGE_H_1724232985

#include <QWidget>

namespace Ui {
class UserManage;
}

namespace WIDGET::SETTING {
class UserManage : public QWidget {
    Q_OBJECT
private:
    Ui::UserManage *ui = nullptr;

public:
    explicit UserManage(QWidget *parent = nullptr);
    ~UserManage();
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_SETTINGUSERMANAGE_H_1724232985

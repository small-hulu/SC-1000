#ifndef WIDGET_DIALOG_MODIFYLOGINUSER_H_1725594623
#define WIDGET_DIALOG_MODIFYLOGINUSER_H_1725594623

#include <QWidget>

#include "controller/CONTROLLER_LoginUser.h"

namespace Ui {
class DEditUser;
}

namespace WIDGET::SETTING {
class DEditUser : public QWidget {
    Q_OBJECT
    using entity_attr = ENTITY::User::attr;

private:
    Ui::DEditUser *ui = nullptr;

public:
    explicit DEditUser(QWidget *parent = nullptr);
    ~DEditUser();

public:
    void Set_user(const ENTITY::User &user);
    void Get_user(ENTITY::User &user);

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void signal_yes();
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_DIALOG_MODIFYLOGINUSER_H_1725594623

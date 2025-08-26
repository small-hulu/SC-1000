#ifndef WIDGET_USERSHOW_H_1724305768
#define WIDGET_USERSHOW_H_1724305768

#include <QTableWidget>
#include <QWidget>

#include "controller/CONTROLLER_LoginUser.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class UserShow;
}

namespace WIDGET::SETTING {

class UserShow : public QWidget {
    Q_OBJECT
    using entity_attr = ENTITY::User::attr;

private:
    static constexpr int Row_Count = 7;

private:
    Ui::UserShow* ui = nullptr;

private:
    QTableWidget* table = nullptr;

private:
    ButtonBottomGroup* m_ui_btnGroup = nullptr;

public:
    UserShow(QWidget* parent = nullptr);
    ~UserShow();

private:
    void new_user();
    void show_range(int start, int end);

protected:
    void showEvent(QShowEvent* event) override;
};

}  // namespace WIDGET::SETTING

#endif  // WIDGET_USERSHOW_H_1724305768

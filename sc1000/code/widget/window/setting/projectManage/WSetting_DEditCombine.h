#ifndef WIDGET_DIALOG_MODIFYLOGINUSER_H_1725500354
#define WIDGET_DIALOG_MODIFYLOGINUSER_H_1725500354

#include <QWidget>

#include "controller/CONTROLLER_LoginUser.h"
#include "widget/Background.h"

namespace Ui {
class DEditCombine;
}

namespace WIDGET::SETTING {
class DEditCombine : public QWidget {
    Q_OBJECT
    using entity_attr = ENTITY::User::attr;

private:
    Ui::DEditCombine *ui = nullptr;

public:
    explicit DEditCombine(QWidget *parent = nullptr);
    ~DEditCombine();

public:
    void Set_listMessage(const QList<QString> &msg);

private:
    void affirm();

protected:
    void showEvent(QShowEvent *event) override;

signals:
    void siganl_yes();
};
}  // namespace WIDGET::SETTING

#endif  // WIDGET_DIALOG_MODIFYLOGINUSER_H_1725500354

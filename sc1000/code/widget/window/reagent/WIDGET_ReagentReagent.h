#ifndef WIDGET_REAGENT_REAGENT_H_1715573215
#define WIDGET_REAGENT_REAGENT_H_1715573215

#include <QPushButton>
#include <QWidget>

#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/modules/WModule_ReagentBinShip.h"

namespace Ui {
class ReagentReagent;
}

namespace WIDGET::REAGENT {
class ReagentTable;

class ReagentReagent : public QWidget {
    Q_OBJECT
private:
    Ui::ReagentReagent *ui                = nullptr;
    ReagentTable       *m_ui_reagentTable = nullptr;
    ReagentBinShip     *m_reagentBin      = nullptr;

private:
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

public:
    explicit ReagentReagent(QWidget *parent = nullptr);
    ~ReagentReagent();

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    void show_reagentTable();
};
}  // namespace WIDGET::REAGENT
#endif  // WIDGET_REAGENT_REAGENT_H_1715573215

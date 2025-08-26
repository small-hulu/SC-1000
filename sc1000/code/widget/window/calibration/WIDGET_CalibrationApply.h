#ifndef WIDGET_CALIBRATIONAPPLY_H_1720068304
#define WIDGET_CALIBRATIONAPPLY_H_1720068304

#include <QPushButton>
#include <QWidget>

#include "apply/WIDGET_CalibrationApplyList.h"
#include "apply/WIDGET_CalibrationApplyMain.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "widget/modules/WModule_ReagentBinSample.h"
#include "widget/modules/WModule_TubeGroup3.h"
#include "widget/modules/WModule_TubeUiItem.h"

namespace Ui {
class CalibrationApply;
}

namespace WIDGET::CALIBRATION {
class CalibrationApply : public QWidget {
    Q_OBJECT
private:
    Ui::CalibrationApply *ui = nullptr;

private:
    ButtonBottomGroup *m_ui_btnGroup       = nullptr;
    ReagentBinSample  *m_ui_reagentBinDial = nullptr;
    TubeGroup3        *m_ui_tubeGroup3     = nullptr;

private:
    CalibrationApplyMain *m_ui_applyMain = nullptr;

public:
    explicit CalibrationApply(QWidget *parent = nullptr);
    ~CalibrationApply();

private:
    void refresh_tubeGroup(int groupNum);

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    void show_applyMain();
};

}  // namespace WIDGET::CALIBRATION
#endif  // WIDGET_CALIBRATIONAPPLY_H_1720068304

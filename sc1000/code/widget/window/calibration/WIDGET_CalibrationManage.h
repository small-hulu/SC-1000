#ifndef WIDGET_CALIBRATIONMANAGE_H_1720068628
#define WIDGET_CALIBRATIONMANAGE_H_1720068628

#include <QPushButton>
#include <QWidget>

#include "manage/WCalibration_CalibrationManageMain.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class CalibrationManage;
}

namespace WIDGET::CALIBRATION {
class CalibrationManage : public QWidget {
    Q_OBJECT
private:
    Ui::CalibrationManage *ui = nullptr;

private:
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

private:
    CalibrationManageMain *m_ui_manageMain = nullptr;

public:
    explicit CalibrationManage(QWidget *parent = nullptr);
    ~CalibrationManage();

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    void show_manageMain();
};
}  // namespace WIDGET::CALIBRATION

#endif  // WIDGET_CALIBRATIONMANAGE_H_1720068628

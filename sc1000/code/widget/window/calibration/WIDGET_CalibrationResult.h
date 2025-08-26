#ifndef WIDGET_CALIBRATIONRESULT_H_1720068534
#define WIDGET_CALIBRATIONRESULT_H_1720068534

#include <QPushButton>
#include <QWidget>

#include "result/WCalibration_CalibrationResultDetail.h"
#include "result/WCalibration_CalibrationResultMain.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"

namespace Ui {
class CalibrationResult;
}

namespace WIDGET::CALIBRATION {
class CalibrationResult : public QWidget {
    Q_OBJECT
private:
    Ui::CalibrationResult *ui;

private:
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

private:
    CalibrationResultMain   *m_ui_resultMain   = nullptr;
    CalibrationResultDetail *m_ui_resultDetail = nullptr;

public:
    explicit CalibrationResult(QWidget *parent = nullptr);
    ~CalibrationResult();

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    void show_resultMain();
    void show_resultDetail();
};
}  // namespace WIDGET::CALIBRATION
#endif  // WIDGET_CALIBRATIONRESULT_H

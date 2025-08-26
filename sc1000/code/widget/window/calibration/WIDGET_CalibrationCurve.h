#ifndef WIDGET_CALIBRATIONCURVE_H_1720613315
#define WIDGET_CALIBRATIONCURVE_H_1720613315

#include <QPushButton>
#include <QWidget>

#include "curve/WCalibration_CalibrationCurveMain.h"
#include "widget/modules/WModule_ButtonBottomGroup.h"
#include "controller/CONTROLLER_Project.h"
namespace Ui {
class CalibrationCurve;
}

namespace WIDGET::CALIBRATION {
class CalibrationCurve : public QWidget {
    Q_OBJECT
private:
    Ui::CalibrationCurve *ui;

    CalibrationCurveMain *m_ui_curveMain = nullptr;

private:
    ButtonBottomGroup *m_ui_btnGroup = nullptr;

public:
    explicit CalibrationCurve(QWidget *parent = nullptr);
    ~CalibrationCurve();

private:
    /// 刷新底部按钮组
    void refresh_bottomBtnList();

private:
    void show_curveMain();
};
}  // namespace WIDGET::CALIBRATION

#endif  // WIDGET_CALIBRATIONCURVE_H_1720613315

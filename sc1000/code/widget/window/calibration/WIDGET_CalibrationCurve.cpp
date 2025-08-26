#include "WIDGET_CalibrationCurve.h"

#include "ui_WIDGET_CalibrationCurve.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {
CalibrationCurve::CalibrationCurve(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationCurve) {
    ui->setupUi(this);

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
    }

    {
        m_ui_btnGroup->Set_refreshFunc(std::bind(&CalibrationCurve::refresh_bottomBtnList, this));
        show_curveMain();
        refresh_bottomBtnList();
    }
}

CalibrationCurve::~CalibrationCurve() {
    m_ui_btnGroup->hide();
    delete ui;
}

void CalibrationCurve::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_ui_curveMain != nullptr) {
        names << QString("生成");
        taskList << std::bind(&CalibrationCurveMain::Btn_test, m_ui_curveMain);
        names << QString("激活");
        taskList << std::bind(&CalibrationCurveMain::Btn_actuvate, m_ui_curveMain);
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void WIDGET::CALIBRATION::CalibrationCurve::show_curveMain() {
    assert(m_ui_curveMain == nullptr);
    m_ui_curveMain = new CalibrationCurveMain(this);
    m_ui_curveMain->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_curveMain->show();
}

}  // namespace WIDGET::CALIBRATION

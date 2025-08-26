#include "WIDGET_CalibrationResult.h"

#include "ui_WIDGET_CalibrationResult.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {
CalibrationResult::CalibrationResult(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationResult) {
    ui->setupUi(this);

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
    }

    {
        m_ui_btnGroup->Set_refreshFunc(std::bind(&CalibrationResult::refresh_bottomBtnList, this));
        show_resultMain();
        refresh_bottomBtnList();
    }
}

CalibrationResult::~CalibrationResult() {
    m_ui_btnGroup->hide();
    delete ui;
}

void CalibrationResult::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_ui_resultMain != nullptr) {
        names << QString("保存");
        taskList << std::bind(&CalibrationResultMain::Btn_save, m_ui_resultMain);
        names << QString("详情");
        taskList << [this]() {
            auto expcal = m_ui_resultMain->Get_curEntity();
            show_resultDetail();
            if (expcal) {
                m_ui_resultDetail->Set_entity(expcal);
            }
        };
        names << QString("删除");
        taskList << std::bind(&CalibrationResultMain::Btn_delete, m_ui_resultMain);
    } else if (m_ui_resultDetail != nullptr) {
        names << QString("返回");
        taskList << std::bind(&CalibrationResult::show_resultMain, this);
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void CalibrationResult::show_resultMain() {
    UTILS::destroy_widget_refPointer(m_ui_resultDetail);

    assert(m_ui_resultMain == nullptr);
    m_ui_resultMain = new CalibrationResultMain(this);
    m_ui_resultMain->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_resultMain->show();
}

void CalibrationResult::show_resultDetail() {
    UTILS::destroy_widget_refPointer(m_ui_resultMain);

    assert(m_ui_resultDetail == nullptr);
    m_ui_resultDetail = new CalibrationResultDetail(this);
    m_ui_resultDetail->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_resultDetail->show();
}
}  // namespace WIDGET::CALIBRATION

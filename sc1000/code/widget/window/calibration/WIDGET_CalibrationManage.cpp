#include "WIDGET_CalibrationManage.h"

#include "ui_WIDGET_CalibrationManage.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {
CalibrationManage::CalibrationManage(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationManage) {
    ui->setupUi(this);

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
    }

    {
        m_ui_btnGroup->Set_refreshFunc(std::bind(&CalibrationManage::refresh_bottomBtnList, this));
        show_manageMain();
        refresh_bottomBtnList();
    }
}

CalibrationManage::~CalibrationManage() {
    m_ui_btnGroup->hide();
    delete ui;
}

void CalibrationManage::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_ui_manageMain != nullptr) {
        names << QString("删除");
        taskList << std::bind(&CalibrationManageMain::Btn_delete, m_ui_manageMain);
        names << QString("编辑");
        taskList << std::bind(&CalibrationManageMain::Btn_edit, m_ui_manageMain);
        names << QString("新增");
        taskList << std::bind(&CalibrationManageMain::Btn_addNew, m_ui_manageMain);
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void CalibrationManage::show_manageMain() {
    assert(m_ui_manageMain == nullptr);
    m_ui_manageMain = new CalibrationManageMain(this);
    m_ui_manageMain->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_manageMain->show();
}
}  // namespace WIDGET::CALIBRATION

#include "WIDGET_QualityResult.h"

#include "result/WQuality_DQualityQuery.h"
#include "ui_WIDGET_QualityResult.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityResult::QualityResult(QWidget *parent) : QWidget(parent), ui(new Ui::QualityResult) {
    ui->setupUi(this);

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
    }

    /// init
    {
        m_ui_btnGroup->Set_refreshFunc(std::bind(&QualityResult::refresh_bottomBtnList, this));
        show_resultMain();
        refresh_bottomBtnList();
    }
}

QualityResult::~QualityResult() {
    m_ui_btnGroup->hide();
    delete ui;
}

void QualityResult::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_ui_resultMain != nullptr) {
        names << QString("删除");
        taskList << std::bind(&QualityResultMain::Entity_uninstall, m_ui_resultMain);
    } else if (m_ui_resultDetail != nullptr) {
        names << QString("返回");
        taskList << std::bind(&QualityResult::show_resultMain, this);
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void QualityResult::show_resultMain() {
    UTILS::destroy_widget_refPointer(m_ui_resultDetail);

    assert(m_ui_resultMain == nullptr);
    m_ui_resultMain = new QualityResultMain(this);
    m_ui_resultMain->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_resultMain->show();
}

void QualityResult::show_resultDetail() {
    UTILS::destroy_widget_refPointer(m_ui_resultMain);

    assert(m_ui_resultDetail == nullptr);
    m_ui_resultDetail = new QualityResultDetail(this);
    m_ui_resultDetail->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_resultDetail->show();
}

}  // namespace WIDGET::QUALITY

#include "WIDGET_QualityAnalyse.h"

#include "ui_WIDGET_QualityAnalyse.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityAnalyse::QualityAnalyse(QWidget *parent) : QWidget(parent), ui(new Ui::QualityAnalyse) {
    ui->setupUi(this);

    /// ui-init
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
    }

    /// init-exe
    {
        m_ui_btnGroup->Set_refreshFunc(std::bind(&QualityAnalyse::refresh_bottomBtnList, this));
        show_analyseMain();
        refresh_bottomBtnList();
    }
}

QualityAnalyse::~QualityAnalyse() {
    m_ui_btnGroup->hide();
    delete ui;
}

void QualityAnalyse::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_analyseMain != nullptr) {
        names << QString("删除");
        taskList << std::bind(&QualityAnalyseMain::Entity_uninstall, m_analyseMain);
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void QualityAnalyse::show_analyseMain() {
    assert(m_analyseMain == nullptr);
    m_analyseMain = new QualityAnalyseMain(this);
    m_analyseMain->setAttribute(Qt::WA_DeleteOnClose);
    m_analyseMain->show();
}
}  // namespace WIDGET::QUALITY

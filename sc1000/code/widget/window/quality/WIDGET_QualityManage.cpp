#include "WIDGET_QualityManage.h"

#include "ui_WIDGET_QualityManage.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityManage::QualityManage(QWidget *parent) : QWidget(parent), ui(new Ui::QualityManage) {
    ui->setupUi(this);

    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
    }

    {
        m_ui_btnGroup->Set_refreshFunc(std::bind(&QualityManage::refresh_bottomBtnList, this));
        show_manageMain();
        refresh_bottomBtnList();
    }
}

QualityManage::~QualityManage() {
    m_ui_btnGroup->hide();
    delete ui;
}

void QualityManage::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_manageMain) {
        names << QString("规则");
        taskList << std::bind(&QualityManage::show_manageRule, this);
        names << QString("删除");
        taskList << std::bind(&QualityManageMain::Entity_uninstall, m_manageMain);
        names << QString("编辑");
        taskList << [this]() {
            auto entity = m_manageMain->Get_curLineEntity();
            if (entity != nullptr) {
                show_manageEdit();
                m_manageEdit->Set_entity(entity);
            } else {
                DIALOG::Create_DWarningDialog("未选中有效项");
            }
        };
        names << QString("手动新增");
        taskList << std::bind(&QualityManage::show_manageDetail, this);
        names << QString("扫码新增");
        taskList << std::bind(&QualityManageMain::Entity_install_byScan, m_manageMain);
    } else if (m_manageDetail) {
        names << QString("返回");
        taskList << std::bind(&QualityManage::show_manageMain, this);
        names << QString("保存");
        taskList << [this]() {
            /// 先做基本的判断
            auto info = m_manageDetail->Get_UiInfo();
            if (info.projectList.isEmpty() == false) {
                m_manageDetail->Btn_save(info);
                DIALOG::Create_DInfoDialog("保存完成");
            } else {
                DIALOG::Create_DWarningDialog("信息输入不全");
            }
        };
    } else if (m_manageEdit) {
        names << QString("返回");
        taskList << std::bind(&QualityManage::show_manageMain, this);
        names << QString("保存");
        taskList << [this]() {
            /// 先做基本的判断
            auto info = m_manageEdit->Get_UiInfo();
            if (info.projectList.isEmpty() == false) {
                m_manageEdit->Btn_save(info);
                DIALOG::Create_DInfoDialog("保存完成");
            } else {
                DIALOG::Create_DWarningDialog("信息输入不全");
            }
        };
    } else if (m_manageRule) {
        names << QString("返回");
        taskList << std::bind(&QualityManage::show_manageMain, this);
        names << QString("保存");
        taskList << []() { qDebug() << __LINE__ << __PRETTY_FUNCTION__; };
    } else {
        assert(false);
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void QualityManage::show_manageMain() {
    UTILS::destroy_widget_refPointer(m_manageDetail);
    UTILS::destroy_widget_refPointer(m_manageEdit);
    UTILS::destroy_widget_refPointer(m_manageRule);

    assert(m_manageMain == nullptr);
    m_manageMain = new QualityManageMain(this);
    m_manageMain->setAttribute(Qt::WA_DeleteOnClose);
    m_manageMain->show();
}

void QualityManage::show_manageDetail() {
    UTILS::destroy_widget_refPointer(m_manageMain);
    UTILS::destroy_widget_refPointer(m_manageEdit);
    UTILS::destroy_widget_refPointer(m_manageRule);

    assert(m_manageDetail == nullptr);
    m_manageDetail = new QualityManageDetail(this);
    m_manageDetail->setAttribute(Qt::WA_DeleteOnClose);
    m_manageDetail->show();
}

void QualityManage::show_manageEdit() {
    UTILS::destroy_widget_refPointer(m_manageMain);
    UTILS::destroy_widget_refPointer(m_manageDetail);
    UTILS::destroy_widget_refPointer(m_manageRule);

    assert(m_manageEdit == nullptr);
    m_manageEdit = new QualityManageDetail(this);
    m_manageEdit->setAttribute(Qt::WA_DeleteOnClose);
    m_manageEdit->show();
}

void QualityManage::show_manageRule() {
    UTILS::destroy_widget_refPointer(m_manageMain);
    UTILS::destroy_widget_refPointer(m_manageDetail);
    UTILS::destroy_widget_refPointer(m_manageEdit);

    assert(m_manageRule == nullptr);
    m_manageRule = new QualityManageRule(this);
    m_manageRule->setAttribute(Qt::WA_DeleteOnClose);
    m_manageRule->show();
}
}  // namespace WIDGET::QUALITY

#include "WIDGET_QualityApply.h"

#include "apply/WQuality_QualityApplyList.h"
#include "apply/WQuality_QualityApplyMain.h"
#include "controller/device/CDevice_DReagentBin.h"
#include "ui_WIDGET_QualityApply.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_ReagentBinSample.h"
#include "widget/modules/WModule_TubeGroup3.h"
#include "widget/modules/WModule_TubeGroup6.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::QUALITY {
QualityApply::QualityApply(QWidget *parent) : QWidget(parent), ui(new Ui::QualityApply) {
    ui->setupUi(this);

    /// ui-init
    {
        new TubeGroup6(ui->widget_top_left);
        m_ui_reagentBinDial = new ReagentBinSample(ui->widget_left_bottom);

        m_ui_tubeGroup3 = new TubeGroup3(ui->widget_top_right);

        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();
    }

    /// connect
    {
        connect(m_ui_reagentBinDial, &ReagentBinSample::signal_curGroup, this,
                &QualityApply::refresh_tubeGroup);
        connect(m_ui_tubeGroup3, &TubeGroup3::signal_tube_select, m_ui_reagentBinDial,
                &ReagentBinSample::Select_onebtn);
    }

    /// init-exe
    {
        refresh_tubeGroup(ReagentBinCache::GroupIdx_Cache);
        m_ui_btnGroup->Set_refreshFunc(std::bind(&QualityApply::refresh_bottomBtnList, this));
        show_applyMain();
        refresh_bottomBtnList();
    }
}

QualityApply::~QualityApply() {
    m_ui_btnGroup->hide();
    delete ui;
}

/**
 * 根据转盘组号，刷新上面的三个管
 */
void QualityApply::refresh_tubeGroup(int groupNum) {
    m_ui_tubeGroup3->Clear_allClicked();
    m_ui_tubeGroup3->Switch_groupMark(groupNum);
    /// todo 设置管的状态
    // m_ui_tubeGroup3->Switch_groupImage({0, 0, 0});
}

/**
 * 本界面比较简单，
 * 就单纯的质控品查看，和申请列表
 */
void QualityApply::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (nullptr != m_ui_applyMain) {
        names << QString("保存");
        taskList << [this]() {
            /**
             * 基本的判断，必填项
             * 判断是否有管
             * 信息有效
             */
            auto info = m_ui_applyMain->Get_UiInfo();
            auto tube = m_ui_tubeGroup3->Get_curTube();
            if (tube) {
                info.location = tube->Get_labelBottomStr();
            }
            if (info.is_valid()) {
                m_ui_applyMain->Btn_save(info);
                DIALOG::Create_DInfoDialog("保存完成");
            } else {
                DIALOG::Create_DWarningDialog("信息输入不全");
            }
        };

        static const QVector<QString> btnNames   = {"启动混匀", "停止混匀"};
        auto                          switch_fun = [this, names]() {
            const auto open =
                COM::IniConfig::instance()[COM::KI::enable_reagentBinSpinLoop].toInt();
            CONTROLLER::CDevice::DReagentBin::Switch_spinLoop(!open);
            auto newNames = names;
            newNames << btnNames[open];
            m_ui_btnGroup->Set_BtnTextList(newNames);
        };

        const auto open = COM::IniConfig::instance()[COM::KI::enable_reagentBinSpinLoop].toInt();
        names << btnNames[open];
        taskList << switch_fun;
    } /* else if (nullptr != m_ui_applyList) {
         names << QString("返回");
         taskList << std::bind(&QualityApply::show_applyMain, this);
         names << QString("卸载");
         taskList << std::bind(&QualityApplyList::Entity_uninstall, m_ui_applyList);
     }*/
    else {
        qWarning() << "ui对应错误";
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void QualityApply::show_applyMain() {
    assert(m_ui_applyMain == nullptr);
    m_ui_applyMain = new QualityApplyMain(ui->widget_right_bottom);
    m_ui_applyMain->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_applyMain->show();
}

}  // namespace WIDGET::QUALITY

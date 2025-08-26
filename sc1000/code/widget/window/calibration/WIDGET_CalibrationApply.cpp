#include "WIDGET_CalibrationApply.h"

#include "controller/device/CDevice_DReagentBin.h"
#include "ui_WIDGET_CalibrationApply.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_ReagentBinSample.h"
#include "widget/modules/WModule_TubeGroup3.h"
#include "widget/modules/WModule_TubeGroup6.h"
#include "widget/modules/WModule_TubeUiItem.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::CALIBRATION {
CalibrationApply::CalibrationApply(QWidget *parent)
    : QWidget(parent), ui(new Ui::CalibrationApply) {
    ui->setupUi(this);

    /// init-ui
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();
        m_ui_btnGroup->show();

        new TubeGroup6(ui->widget_top_left);
        m_ui_tubeGroup3     = new TubeGroup3(ui->widget_top_right);
        m_ui_reagentBinDial = new ReagentBinSample(ui->widget_left_bottom);
        connect(m_ui_tubeGroup3, &TubeGroup3::signal_tube_select, m_ui_reagentBinDial,
                &ReagentBinSample::Select_onebtn);
    }

    /// connect
    {
        connect(m_ui_reagentBinDial, &ReagentBinSample::signal_curGroup, this,
                &CalibrationApply::refresh_tubeGroup);

        connect(m_ui_tubeGroup3, &TubeGroup3::signal_clicked, this, [this](entity_ui_tube tube) {
            if (m_ui_applyMain) {
                m_ui_applyMain->Set_tubeIndex(tube->Get_labelBottomStr());
            }
        });
    }

    /// init-exe
    {
        refresh_tubeGroup(ReagentBinCache::GroupIdx_Cache);
        m_ui_btnGroup->Set_refreshFunc(std::bind(&CalibrationApply::refresh_bottomBtnList, this));
        show_applyMain();
        refresh_bottomBtnList();
    }
}

CalibrationApply::~CalibrationApply() {
    m_ui_btnGroup->hide();
    delete ui;
}

void CalibrationApply::refresh_tubeGroup(int groupNum) {
    m_ui_tubeGroup3->Clear_allClicked();
    m_ui_tubeGroup3->Switch_groupMark(groupNum);
    /// todo 设置管的状态
    // m_ui_tubeGroup3->Switch_groupImage({0, 0, 0});
}

void CalibrationApply::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_ui_applyMain != nullptr) {
        names << QString("保存");
        taskList << [this]() {
            auto info = m_ui_applyMain->Get_uiInfo();
            if (info.Is_valid() == false) {
                DIALOG::Create_DWarningDialog("信息不全，请填写");
                return;
            }

            m_ui_applyMain->Btn_save(info);
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
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void CalibrationApply::show_applyMain() {
    assert(m_ui_applyMain == nullptr);
    m_ui_applyMain = new CalibrationApplyMain(ui->widget_right_bottom);
    m_ui_applyMain->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_applyMain->show();
}

}  // namespace WIDGET::CALIBRATION

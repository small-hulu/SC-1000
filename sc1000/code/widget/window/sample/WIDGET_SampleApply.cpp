#include "WIDGET_SampleApply.h"

#include <QDebug>

#include "apply/WSample_SampleApplyMain.h"
#include "apply/WSample_SampleApplyPatient.h"
#include "controller/CONTROLLER_Sample.h"
#include "controller/CONTROLLER_SampleApply.h"
#include "controller/device/CDevice_DReagentBin.h"
#include "entity/ENTITY_ESample.h"
#include "ui_WIDGET_SampleApply.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_TubeGroup3.h"
#include "widget/modules/WModule_TubeGroup6.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SAMPLE {

SampleApply::SampleApply(QWidget* parent) : QWidget(parent), ui(new Ui::SampleApply) {
    ui->setupUi(this);

    /// init-ui
    {
        ui->lab_num_edit->setText("");
        ui->lab_project_edit->setText("");
    }

    /// 动态 ui
    {
        m_ui_btnGroup = ButtonBottomGroup::Get();  // 按钮组
        m_ui_btnGroup->show();

        new TubeGroup6(ui->widget_left_up);  // 6个试剂管状态图标，单纯的展示
        m_ui_reagentBinDial = new ReagentBinSample(ui->widget_left_down);  // 转盘

        m_ui_tubeGroup3 = new TubeGroup3(ui->widget_up_left);  // 3个可选试剂管
        connect(m_ui_tubeGroup3, &TubeGroup3::signal_tube_select, m_ui_reagentBinDial,
                &ReagentBinSample::Select_onebtn);
    }

    /// lis
    {
        lis_con = &LIS::Lis::instance();
        connect(lis_con, &LIS::Lis::signal_lisData, this, &SampleApply::Bidirectional_Lis_Slot);
    }

    /// connect
    {
        connect(m_ui_reagentBinDial, &ReagentBinSample::signal_curGroup, this,
                &SampleApply::refresh_tubeGroup);  // 转动转盘更新3个试剂管
    }

    /// init-exe
    {
        refresh_tubeGroup(ReagentBinCache::GroupIdx_Cache);
        m_ui_btnGroup->Set_refreshFunc(std::bind(&SampleApply::refresh_bottomBtnList, this));
        show_applyMain();
        refresh_bottomBtnList();
    }

    startTimer(Sync_IntervalTime);
}

SampleApply::~SampleApply() {
    m_ui_btnGroup->hide();
    delete ui;
}

/**
 * 根据转盘组号，刷新上面的三个管
 */
void SampleApply::refresh_tubeGroup(int groupNum) {
    m_ui_tubeGroup3->Clear_allClicked();
    m_ui_tubeGroup3->Switch_groupMark(groupNum);
}

/**
 * 将 info 的信息展示到 ui 中
 */
void SampleApply::refresh_into_toUi() {
    ui->lab_num_edit->setText(m_info.userEditNumber);
    ui->lab_project_edit->setText(m_info.projectList.join(UTILS::join_separator));
    if (m_info.lis_sign) {
        ui->lab_project_edit->setText(m_info.project);
    }
}

// 申请页面底部按钮的生成并设置对应的槽函数
void SampleApply::refresh_bottomBtnList() {
    QList<QString>                   names;
    QList<std::function<void(void)>> taskList;

    if (false) {
    } else if (m_ui_applyMain) {
        names << QString("保存");
        taskList << std::bind(&SampleApply::save_experiment, this);

        names << QString("患者信息");
        taskList << [this]() {
            auto tube = m_ui_tubeGroup3->Get_curTube();
            if (tube) {
                QString tube_num = tube->Get_labelBottomStr();
                m_info.location  = tube_num;
                m_info.patientInfo.Reset();
                show_applyPatient(tube_num);
            } else if (tube == nullptr) {
                DIALOG::Create_DWarningDialog("请选择试管");
            }
        };
        names << QString("LIS获取");
        taskList << std::bind(&SampleApply::lis_obtain, this);
        names << QString("批量申请");
        taskList << std::bind(&SampleApplyMain::Multiple_Apply, m_ui_applyMain);
        names << QString("释放所有");
        taskList << std::bind(&SampleApply::releaseAll, this);

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
    } else if (m_ui_applyPatient) {
        names << QString("返回");
        taskList << std::bind(&SampleApply::show_applyMain, this);
        names << QString("确定");
        taskList << [this]() {
            m_ui_applyPatient->Btn_confirm(m_info);
            m_info.Reset();
            DIALOG::Create_DInfoDialog("保存完成");
        };
    }

    m_ui_btnGroup->Set_BtnList(names, taskList);
}

void SampleApply::save_experiment() {
    auto tube = m_ui_tubeGroup3->Get_curTube();
    if (tube == nullptr) {
        DIALOG::Create_DWarningDialog("未选择转盘孔位");
        return;
    }
    m_ui_applyMain->Get_UiInfo(m_info);
    m_info.location = tube->Get_labelBottomStr();

    if (m_info.projectList.isEmpty()) {
        DIALOG::Create_DWarningDialog("未选择执行项目");
        return;
    }

    if (m_info.lab_barcode == "样本条码") {
        m_info.applyCount = 1;
        m_ui_applyMain->Btn_save(m_info);
    }
    // 批量申请
    else if (m_info.lab_barcode == "申请个数") {
        m_info.applyCount = m_info.barCode.toInt();
        for (int i = 0; i < qMin(m_info.applyCount, 30); i++) {
            QString curLocation = tube->Get_labelBottomStr();
            auto    idx         = (int)Type::Get_TubeSpaceMark(curLocation) + i;
            m_info.location     = Type::Get_TubeSpaceMark((Type::ETubeSpaceMark)(idx));
            m_ui_applyMain->Btn_save(m_info);
        }
    }
    refresh_into_toUi();
    m_info.Reset();
    // update sample icon
    m_ui_reagentBinDial->Update_dial();

    DIALOG::Create_DInfoDialog("保存完成");
}

void SampleApply::show_applyMain() {
    UTILS::destroy_widget_refPointer(m_ui_applyPatient);

    assert(m_ui_applyMain == nullptr);
    m_ui_applyMain = new SampleApplyMain(ui->widget_right_down);
    m_ui_applyMain->setAttribute(Qt::WA_DeleteOnClose);
    m_ui_applyMain->show();
}

void SampleApply::show_applyPatient(QString tube_num) {
    UTILS::destroy_widget_refPointer(m_ui_applyMain);
    assert(m_ui_applyPatient == nullptr);
    m_ui_applyPatient = new SampleApplyPatient(ui->widget_right_down);
    m_ui_applyPatient->setAttribute(Qt::WA_DeleteOnClose);

    using namespace ENTITY;
    auto& control = CONTROLLER::Sample::instance();
    auto  entity  = control.Get_isLoadByLocation(tube_num);
    if (entity) {
        m_info.patientInfo.name       = entity->table[ESample::attr::patient_name];
        m_info.patientInfo.sex        = entity->table[ESample::attr::patient_sex];
        m_info.patientInfo.ageNum     = entity->table[ESample::attr::patient_ageNum];
        m_info.patientInfo.ageUnit    = entity->table[ESample::attr::patient_ageUnit];
        m_info.patientInfo.patientId  = entity->table[ESample::attr::patient_patientId];
        m_info.patientInfo.department = entity->table[ESample::attr::patient_department];
        m_info.patientInfo.bedId      = entity->table[ESample::attr::patient_bedId];
        m_info.patientInfo.inspection = entity->table[ESample::attr::patient_inspection];
        m_info.patientInfo.verifier   = entity->table[ESample::attr::patient_verifier];
        m_info.patientInfo.auditor    = entity->table[ESample::attr::patient_auditor];
        m_info.patientInfo.remark     = entity->table[ESample::attr::patient_remark];
    }
    m_ui_applyPatient->Set_info(m_info);
    m_ui_applyPatient->show();
}

void SampleApply::lis_obtain() {
    // 根据lis配置信息做初步判断，双向lis下可用，不是双向lis进行弹窗
    auto tube = m_ui_tubeGroup3->Get_curTube();
    if (tube) {
        lis_con->setMode(LIS::Lis::Mode::Bidirectional);
        m_ui_applyMain->Get_UiInfo(m_info);
        QString code_barcode = m_info.barCode;
        LIS::Lis_SC1000::Send_singleMessage(m_info.barCode);
    } else {
        DIALOG::Create_DWarningDialog("请选择一个试管");
    }
}

void SampleApply::releaseAll() {
    auto dialog = DIALOG::Create_DIsVerify("确认需要释放所有？");
    connect(dialog, &DIALOG::DIsVerify::signal_yes, this, [this]() {
        auto sampleList = CONTROLLER::Sample::instance().Get_isLoadList();
        for (auto&& sample : sampleList) {
            sample->table[CONTROLLER::Sample::entity_attr::isLoad] = "0";
            sample->Update_toDB();
        }
        auto sampleApplyList = CONTROLLER::SampleApply::instance().Select_all_fromDB();
        for (auto&& apply : sampleApplyList) {
            apply->Delete_toDB();
        }

        m_ui_reagentBinDial->Update_dial();
        DIALOG::Create_DInfoDialog("完成");
    });
}

void SampleApply::Bidirectional_Lis_Slot(const QByteArray& bytes) {
    QString     data   = QString::fromUtf8(bytes).trimmed();
    QStringList fields = data.split(',');
    if (fields.size() < 5) {
        qWarning() << "Data format is incorrect:" << data;
        return;
    }
    QString     location   = fields[1];
    QString     patientId  = fields[2];
    QString     sampleType = fields.last();
    QStringList testItems  = fields.mid(3, fields.size() - 4);

    auto tube             = m_ui_tubeGroup3->Get_curTube();
    m_info.userEditNumber = patientId.toInt();
    m_info.barCode        = patientId.toInt();
    m_info.detectType     = sampleType;
    m_info.project        = testItems.first();  // 默认先处理一个项目的lis消息
    m_info.lis_sign       = true;
    if (tube) {
        m_info.location = tube->Get_labelBottomStr().toInt();
        m_ui_applyMain->Btn_save(m_info);
        refresh_into_toUi();
        m_info.Reset();
        DIALOG::Create_DInfoDialog("保存完成");
    }
}

void SampleApply::timerEvent(QTimerEvent* event) {
    m_ui_reagentBinDial->Update_dial();
}

}  // namespace WIDGET::SAMPLE

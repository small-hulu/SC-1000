#include "WSample_SampleApplyMain.h"

#include "controller/CONTROLLER_Config.h"
#include "controller/CONTROLLER_Project.h"
#include "controller/CONTROLLER_SampleApply.h"
#include "global/GLOBAL_Type.h"
#include "qrcode/QRCODE.hpp"
#include "ui_WSample_SampleApplyMain.h"
#include "widget/dialog/DIALOG.h"
#include "widget/modules/WModule_TablePageTurnSwitch.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SAMPLE {
SampleApplyMain::SampleApplyMain(QWidget *parent) : QWidget(parent), ui(new Ui::SampleApplyMain) {
    ui->setupUi(this);

    /// 基础组件
    {
        ui->edit_msg->setEnabled(false);
        ui->edit_msg_down->setEnabled(false);

        m_ui_switchList = new SwitchList(ui->widget_type);
        m_ui_switchList->Set_msgList(Type::TypeList_of_detect());

        m_ui_switchList_tubetype = new SwitchList(ui->widget_type_2);
        m_ui_switchList_tubetype->Set_msgList(Type::TypeList_of_tube_type());
    }

    /// 项目列表 和 组合项目列表
    {
        m_ui_tableSelect = new TableSelect(ui->widget_down);
        m_ui_tableSelect->Builder(Up_Row_Count, Up_Col_Count);
        m_ui_tableSelect_Merge = new TableSelectSingle(ui->widget_down_2);
        m_ui_tableSelect_Merge->Builder(Down_Row_Count, Down_Col_Count);

        connect(m_ui_tableSelect_Merge, &TableSelectSingle::signal_cellSelected, this,
                [this](const QString &msg) {
                    auto fileCon = CONTROLLER::Config::ProjectCombine{};
                    auto group   = fileCon.Get_CombineByName(msg);
                    m_ui_tableSelect->Set_tableClickedList(group.projectList);
                });
    }

    /// 翻页按钮的信号槽
    {
        connect(ui->btn_left, &QPushButton::clicked, this, [this]() {
            const int curPage = ui->edit_msg->text().toInt();
            check_page(curPage - 1);
        });
        connect(ui->btn_right, &QPushButton::clicked, this, [this]() {
            const int curPage = ui->edit_msg->text().toInt();
            check_page(curPage + 1);
        });

        connect(ui->btn_left_down, &QPushButton::clicked, this, [this]() {
            const int curPage = ui->edit_msg_down->text().toInt();
            check_page_down(curPage - 1);
        });
        connect(ui->btn_right_down, &QPushButton::clicked, this, [this]() {
            const int curPage = ui->edit_msg_down->text().toInt();
            check_page_down(curPage + 1);
        });
    }

    /// 项目和组合项目的翻页
    {
        //xsw
        connect(this, &SampleApplyMain::signal_curRange, this, &SampleApplyMain::show_range);
        auto      fileCon = CONTROLLER::Create_ProjectsFolderController();
        auto      names   = fileCon.Get_ProjectList();
        const int all     = names.size();
        Build_By_ItemCount(all, Up_Row_Count * Up_Col_Count);
        this->signal_curRange(0, qMin(all, Up_Row_Count * Up_Col_Count));

        connect(this, &SampleApplyMain::signal_curRange_down, this,
                &SampleApplyMain::show_range_down);
        auto com = CONTROLLER::Config::ProjectCombine{};
        m_ui_tableSelect_Merge->Set_tableText(com.Get_CombineNames());
        auto      names_down = com.Get_CombineNames();
        const int all_down   = names_down.size();
        Build_By_ItemCount_down(all_down, Down_Row_Count * Down_Col_Count);
        this->signal_curRange_down(0, qMin(all_down, Down_Row_Count * Down_Col_Count));
    }

    /// 扫码功能监控
    {
        auto &qrcode = QRCODE::SerialQrcode::instance();
        connect(&qrcode, &QRCODE::SerialQrcode::signal_qrcode_originalData, this,
                [this](const QByteArray &bytes, int type) {
                    auto qrcodeType = (QRCODE::QrcodeType)type;
                    /// 过滤掉特种编码
                    if (qrcodeType == QRCODE::QrcodeType::reagent ||
                        qrcodeType == QRCODE::QrcodeType::consumable ||
                        qrcodeType == QRCODE::QrcodeType::calibration ||
                        qrcodeType == QRCODE::QrcodeType::quality) {
                        DIALOG::Create_DWarningDialog("条码错误");
                        return;
                    }
                    ui->edit_barcode->setText(bytes);
                });
    }
}

SampleApplyMain::~SampleApplyMain() {
    delete ui;
}

SampleApplyMain::UiInfo SampleApplyMain::Get_UiInfo() {
    UiInfo info;
    info.number      = ui->edit_num->text();
    info.barCode     = ui->edit_barcode->text();
    info.detectType  = m_ui_switchList->Get_curMessage();
    info.sampleTube  = m_ui_switchList_tubetype->Get_curMessage();
    info.projectList = m_ui_tableSelect->Get_tableCheckedString();
    return info;
}

void SampleApplyMain::Get_UiInfo(InfoSample::ApplyInfo &info) {
    info.lab_barcode            = ui->lab_barcode->text();
    info.userEditNumber         = ui->edit_num->text();
    info.barCode                = ui->edit_barcode->text();
    info.sampleTube             = m_ui_switchList_tubetype->Get_curMessage();
    info.detectType             = m_ui_switchList->Get_curMessage();
    info.projectList            = m_ui_tableSelect->Get_tableCheckedString();
    info.projectList_MerageName = m_ui_tableSelect_Merge->Get_curChecked();
    info.emergency_sign         = priority;
}

SampleApplyMain::entity_item SampleApplyMain::Btn_save(const InfoSample::ApplyInfo &info) {
    auto sync_data = [&](CONTROLLER::Sample::entity_item entity) {
        entity->table[entity_attr::state] =
            QString::number((int)CONTROLLER::Sample::entity_type::TubeState::Apply);
        entity->table[entity_attr::number]         = info.userEditNumber;
        entity->table[entity_attr::isLoad]         = "1";
        entity->table[entity_attr::location]       = info.location;
        entity->table[entity_attr::detectType]     = info.detectType;
        entity->table[entity_attr::sampleTube]     = info.sampleTube;
        entity->table[entity_attr::qrcode]         = info.barCode;
        entity->table[entity_attr::emergency_sign] = info.emergency_sign;
        entity->table[entity_attr::project]        = info.projectList.join(UTILS::join_separator);
        if (info.lis_sign) {
            entity->table[entity_attr::project] = info.project;
        }
        auto &patient                                  = info.patientInfo;
        entity->table[entity_attr::patient_name]       = patient.name;
        entity->table[entity_attr::patient_ageNum]     = patient.ageNum;
        entity->table[entity_attr::patient_ageUnit]    = patient.ageUnit;
        entity->table[entity_attr::patient_sex]        = patient.sex;
        entity->table[entity_attr::patient_patientId]  = patient.patientId;
        entity->table[entity_attr::patient_department] = patient.department;
        entity->table[entity_attr::patient_bedId]      = patient.bedId;
        entity->table[entity_attr::patient_inspection] = patient.inspection;
        entity->table[entity_attr::patient_verifier]   = patient.verifier;
        entity->table[entity_attr::patient_auditor]    = patient.auditor;
        entity->table[entity_attr::patient_remark]     = patient.remark;
    };

    /// 根据位置信息查看是否有缓存
    auto entity = CONTROLLER::Sample::instance().Get_isLoadByLocation(info.location);
    if (entity) {
        sync_data(entity);
        entity->Update_toDB();
    } else {
        entity = CONTROLLER::Sample::Create_oneEntityItem();
        sync_data(entity);
        entity->Insert_toDB();
    }

    /// 每次都要生成一个申请列表的信息
    using apply_attr                            = CONTROLLER::SampleApply::entity_attr;
    auto &&sampleApply                          = CONTROLLER::SampleApply::instance();
    auto   applyEntity                          = sampleApply.Create_oneEntityItem();
    applyEntity->table[apply_attr::idSample]    = entity->Get_idString();
    applyEntity->table[apply_attr::projectList] = entity->table[entity_attr::project];
    applyEntity->Insert_toDB();

    return entity;
}

SampleApplyMain::entity_item SampleApplyMain::Btn_save_Merage(const InfoSample::ApplyInfo &info) {
    auto entity                                    = CONTROLLER::Sample::Create_oneEntityItem();
    entity->table[entity_attr::number]             = info.userEditNumber;
    entity->table[entity_attr::isLoad]             = "1";
    entity->table[entity_attr::location]           = info.location;
    entity->table[entity_attr::detectType]         = info.detectType;
    entity->table[entity_attr::qrcode]             = info.barCode;
    entity->table[entity_attr::project]            = info.project_Merage;
    auto &patient                                  = info.patientInfo;
    entity->table[entity_attr::patient_name]       = patient.name;
    entity->table[entity_attr::patient_ageNum]     = patient.ageNum;
    entity->table[entity_attr::patient_ageUnit]    = patient.ageUnit;
    entity->table[entity_attr::patient_sex]        = patient.sex;
    entity->table[entity_attr::patient_patientId]  = patient.patientId;
    entity->table[entity_attr::patient_department] = patient.department;
    entity->table[entity_attr::patient_bedId]      = patient.bedId;
    entity->table[entity_attr::patient_inspection] = patient.inspection;
    entity->table[entity_attr::patient_verifier]   = patient.verifier;
    entity->table[entity_attr::patient_auditor]    = patient.auditor;
    entity->table[entity_attr::patient_remark]     = patient.remark;

    entity->Insert_toDB();
    ui->edit_num->setText(QString(info.userEditNumber));

    using sampleApplt_attr = CONTROLLER::SampleApply::entity_attr;
    auto sampleApply       = CONTROLLER::SampleApply::Create_oneEntityItem();
    sampleApply->table[sampleApplt_attr::idSample]    = entity->Get_idString();
    sampleApply->table[sampleApplt_attr::projectList] = entity->table[entity_attr::project];
    sampleApply->Insert_toDB();

    return entity;
}

void SampleApplyMain::on_pushButton_emergency_clicked() {
    emergency_sign = !emergency_sign;
    if (emergency_sign) {
        ui->pushButton_emergency->setStyleSheet(
            "QPushButton {"
            "   background-image: url(:/beijing/emergency.png);"
            "   background-repeat: no-repeat;"
            "   background-position: center;"
            "   border: none;"
            "   padding: 0px;"
            "   min-width: 103px;"
            "   min-height: 50px;"
            "}");
        priority = "急诊";
    } else {
        ui->pushButton_emergency->setStyleSheet(
            "QPushButton {"
            "   background-image: url(:/beijing/emergency_none.png);"
            "   background-repeat: no-repeat;"
            "   background-position: center;"
            "   border: none;"
            "   padding: 0px;"
            "   min-width: 103px;"
            "   min-height: 50px;"
            "}");
        priority = "常规";
    }
}

void SampleApplyMain::check_page(int page) {
    if (page <= 0 || (page - 1) * each > all) {
        return;
    }

    ui->edit_msg->setText(QString::number(page));
    const int start = (page - 1) * each + 1;
    const int end   = qMin(page * each, all);
    emit      signal_curRange(start, end);
}
void SampleApplyMain::check_page_down(int page) {
    if (page <= 0 || (page - 1) * each_down > all_down) {
        return;
    }

    ui->edit_msg_down->setText(QString::number(page));
    const int start = (page - 1) * each_down + 1;
    const int end   = qMin(page * each_down, all_down);
    emit      signal_curRange_down(start, end);
}
void SampleApplyMain::show_range(int start, int end) {
    auto fileCon = CONTROLLER::Create_ProjectsFolderController();
    auto names   = fileCon.Get_ProjectList();
    if (start <= 0 && end <= 0) {
        // pass
    } else {
        names = names.mid(start, end - start + 1);
    }
    m_ui_tableSelect->Set_tableText(names);
    auto isLoadList     = CONTROLLER::Reagent::instance().Select_isLoadList();
    auto curProjectList = CONTROLLER::Reagent::instance().Filter_StringList_bybyAttribute(
        isLoadList, ENTITY::EReagent::attr::project);
    m_ui_tableSelect->Set_tableEnableList(curProjectList);
}

void SampleApplyMain::show_range_down(int start, int end) {
    auto com   = CONTROLLER::Config::ProjectCombine{};
    auto names = com.Get_CombineNames();
    if (start <= 0 && end <= 0) {
        // pass
    } else {
        names = names.mid(start, end - start + 1);
    }
    m_ui_tableSelect_Merge->Set_tableText(names);
}

void SampleApplyMain::Build_By_ItemCount(int all, int each) {
    this->all  = all;
    this->each = each;
    ui->edit_msg->setText("1");
}

void SampleApplyMain::Build_By_ItemCount_down(int all_down, int each_down) {
    this->all_down  = all_down;
    this->each_down = each_down;
    ui->edit_msg_down->setText("1");
}

void SampleApplyMain::Multiple_Apply() {
    if (ui->lab_barcode->text() == "样本条码") {
        ui->lab_barcode->setText("申请个数");
        ui->edit_barcode->setFixedWidth(100);
    } else {
        ui->lab_barcode->setText("样本条码");
        ui->edit_barcode->setFixedWidth(304);
    }
}
void SampleApplyMain::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::SAMPLE

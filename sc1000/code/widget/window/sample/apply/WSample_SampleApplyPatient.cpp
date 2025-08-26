#include "WSample_SampleApplyPatient.h"

#include "global/GLOBAL_Type.h"
#include "ui_WSample_SampleApplyPatient.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SAMPLE {
SampleApplyPatient::SampleApplyPatient(QWidget *parent)
    : QWidget(parent), ui(new Ui::SampleApplyPatient) {
    ui->setupUi(this);

    /// ui
    UTILS::resize_layout_stretch(ui->verticalLayout, {2, 2, 2, 5});

    /// 选项类
    {
        m_ui_ageUnit = new SwitchList(ui->widget_age_unit);
        m_ui_ageUnit->Set_msgList(Type::TypeList_of_ageUnit());

        m_ui_sex = new SwitchList(ui->widget_sex);
        m_ui_sex->Set_msgList(Type::TypeList_of_sex());
    }
}

SampleApplyPatient::~SampleApplyPatient() {
    delete ui;
}

void SampleApplyPatient::Set_info(const InfoSample::ApplyInfo &info) {
    auto &patient = info.patientInfo;
    ui->edit_name->setText(patient.name);
    ui->edit_age_val->setText(patient.ageNum);
    ui->edit_patientID->setText(patient.patientId);
    ui->edit_department->setText(patient.department);
    ui->edit_bedID->setText(patient.bedId);
    ui->edit_inspection->setText(patient.inspection);
    ui->edit_verifier->setText(patient.verifier);
    ui->edit_auditor->setText(patient.auditor);

    m_ui_sex->Set_curMessage(patient.sex);
    m_ui_ageUnit->Set_curMessage(patient.ageUnit);
}

void SampleApplyPatient::Btn_confirm(InfoSample::ApplyInfo &info) {
    auto &patient = info.patientInfo;

    patient.name       = ui->edit_name->text();
    patient.ageNum     = ui->edit_age_val->text();
    patient.patientId  = ui->edit_patientID->text();
    patient.department = ui->edit_department->text();
    patient.bedId      = ui->edit_bedID->text();
    patient.inspection = ui->edit_inspection->text();
    patient.verifier   = ui->edit_verifier->text();
    patient.auditor    = ui->edit_verifier->text();
    patient.sex        = m_ui_sex->Get_curMessage();
    patient.ageUnit    = m_ui_ageUnit->Get_curMessage();

    // 将数据根据位置存数据库
    using namespace ENTITY;
    auto &control = CONTROLLER::Sample::instance();
    auto  entity  = control.Get_isLoadByLocation(info.location);
    if (entity) {
        entity->table[ESample::attr::patient_name]       = patient.name;
        entity->table[ESample::attr::patient_sex]        = patient.sex;
        entity->table[ESample::attr::patient_ageNum]     = patient.ageNum;
        entity->table[ESample::attr::patient_ageUnit]    = patient.ageUnit;
        entity->table[ESample::attr::patient_patientId]  = patient.patientId;
        entity->table[ESample::attr::patient_department] = patient.department;
        entity->table[ESample::attr::patient_bedId]      = patient.bedId;
        entity->table[ESample::attr::patient_inspection] = patient.inspection;
        entity->table[ESample::attr::patient_verifier]   = patient.verifier;
        entity->table[ESample::attr::patient_auditor]    = patient.auditor;
        entity->table[ESample::attr::patient_remark]     = patient.remark;
        entity->Update_toDB();
    }
}

void SampleApplyPatient::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::SAMPLE

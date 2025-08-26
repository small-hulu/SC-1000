#include "WIDGET_SettingHospitalManage.h"

#include "hospitalManage/WSetting_HospitalConfig.h"
#include "ui_WIDGET_SettingHospitalManage.h"

namespace WIDGET::SETTING {
HospitalManage::HospitalManage(QWidget *parent) : QWidget(parent), ui(new Ui::HospitalManage) {
    ui->setupUi(this);

    new HospitalConfig(this);
}

HospitalManage::~HospitalManage() {
    delete ui;
}
}  // namespace WIDGET::SETTING

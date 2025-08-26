#include "WIDGET_SettingLISManage.h"

#include "lisManage/WSetting_LISConfig.h"
#include "ui_WIDGET_SettingLISManage.h"

namespace WIDGET::SETTING {
LISManage::LISManage(QWidget *parent) : QWidget(parent), ui(new Ui::LISManage) {
    ui->setupUi(this);

    new LISConfig(this);
}

LISManage::~LISManage() {
    delete ui;
}
}  // namespace WIDGET::SETTING

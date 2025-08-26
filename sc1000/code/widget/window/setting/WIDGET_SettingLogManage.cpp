#include "WIDGET_SettingLogManage.h"

#include "logManage/WSetting_LogShow.h"
#include "ui_WIDGET_SettingLogManage.h"

namespace WIDGET::SETTING {
LogManage::LogManage(QWidget *parent) : QWidget(parent), ui(new Ui::LogManage) {
    ui->setupUi(this);

    new LogShow(this);
}

LogManage::~LogManage() {
    delete ui;
}
}  // namespace WIDGET::SETTING

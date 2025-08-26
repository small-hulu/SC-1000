#include "WIDGET_LogManage.h"

#include "ui_WIDGET_LogManage.h"

namespace WIDGET::ALARM {
LogManage::LogManage(QWidget *parent) : QWidget(parent), ui(new Ui::LogManage) {
    ui->setupUi(this);

    ui->pushButton->setText(__FILE__);
}

LogManage::~LogManage() {
    delete ui;
}
}  // namespace WIDGET::ALARM

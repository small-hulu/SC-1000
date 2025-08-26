#include "WIDGET_SettingPrintManage.h"

#include "printManage/WSetting_PaperPrintParams.h"
#include "ui_WIDGET_SettingPrintManage.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::SETTING {
PrintManage::PrintManage(QWidget *parent) : QWidget(parent), ui(new Ui::PrintManage) {
    ui->setupUi(this);

    new PaperPrintParams(this);
}

PrintManage::~PrintManage() {
    delete ui;
}

void PrintManage::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET::SETTING

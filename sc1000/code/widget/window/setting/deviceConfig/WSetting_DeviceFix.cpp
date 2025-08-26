#include "WSetting_DeviceFix.h"

#include "ui_WSetting_DeviceFix.h"

namespace WIDGET::SETTING {
DeviceFix::DeviceFix(QWidget *parent) : QWidget(parent), ui(new Ui::DeviceFix) {
    ui->setupUi(this);
}

DeviceFix::~DeviceFix() {
    delete ui;
}
}  // namespace WIDGET::SETTING

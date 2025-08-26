#include "WIDGET_HelpAboutDevice.h"

#include "aboutDevice/WHelp_MCUUpgrade.h"
#include "aboutDevice/WHelp_MCUVersion.h"
#include "ui_WIDGET_HelpAboutDevice.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET::HELP {

AboutDevice::AboutDevice(QWidget *parent) : QWidget(parent), ui(new Ui::AboutDevice) {
    ui->setupUi(this);
    new MCUVersion(this);
}

AboutDevice::~AboutDevice() {
    delete ui;
}
}  // namespace WIDGET::HELP

#include "WIDGET_HelpUpgrade.h"

#include "aboutDevice/WHelp_MCUUpgrade.h"
#include "ui_WIDGET_HelpUpgrade.h"

namespace WIDGET::HELP {
HelpUpgrade::HelpUpgrade(QWidget *parent) : QWidget(parent), ui(new Ui::HelpUpgrade) {
    ui->setupUi(this);

    new MCUUpgrade(this);
}

HelpUpgrade::~HelpUpgrade() {
    delete ui;
}
}  // namespace WIDGET::HELP

#include "WSetting_SettingOption.h"

#include "ui_WSetting_SettingOption.h"

namespace WIDGET::SETTING {
SettingOption::SettingOption(QWidget *parent) : QWidget(parent), ui(new Ui::SettingOption) {
    ui->setupUi(this);
}

SettingOption::SettingOption(QString btnText, QWidget *parent) : SettingOption(parent) {
    ui->pushButton->setText(btnText);

    connect(ui->pushButton, &QPushButton::clicked, this, &SettingOption::signal_selected);
}

SettingOption::~SettingOption() {
    delete ui;
}
}  // namespace WIDGET::SETTING

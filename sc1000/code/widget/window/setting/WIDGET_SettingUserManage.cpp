#include "WIDGET_SettingUserManage.h"

#include "ui_WIDGET_SettingUserManage.h"
#include "userManage/WSetting_UserShow.h"

namespace WIDGET::SETTING {
UserManage::UserManage(QWidget *parent) : QWidget(parent), ui(new Ui::UserManage) {
    ui->setupUi(this);

    new UserShow(this);
}

UserManage::~UserManage() {
    delete ui;
}
}  // namespace WIDGET::SETTING

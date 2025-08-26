#include "WIDGET_Login.h"

#include <QTimer>

#include "controller/CONTROLLER_LoginUser.h"
#include "ui_WIDGET_Login.h"
#include "widget/dialog/DIALOG.h"
#include "widget/utils/UTILS.hpp"

namespace WIDGET {

Login::Login(QWidget *parent) : QWidget(parent), ui(new Ui::Login{}) {
    ui->setupUi(this);
    setAttribute(Qt::WA_StyledBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    {
        connect(ui->btn_login, &QPushButton::clicked, this, &Login::check_log);
        // QTimer::singleShot(20, this, &Login::signal_loginOk);
    }
}

Login::~Login() {
    delete ui;
}

void Login::check_log() {
    const QString name     = ui->lineEdit_username->text();
    const QString password = ui->LineEdit_pd->text();

    sLog_test() << QString("尝试登录账号[%1]密码[%2]").arg(name, password);

    if (CONTROLLER::LoginUser::instance().Try_login(name, password)) {
        emit this->signal_loginOk();
    } else {
        DIALOG::Create_DWarningDialog("账号或密码错误，请重新登录");
    }
}

void Login::showEvent(QShowEvent *event) {
    UTILS::resize_widget_fromParent(this);
    QWidget::showEvent(event);
}
}  // namespace WIDGET

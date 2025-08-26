#include "MTop_AppState.h"

#include <QApplication>
#include <QDebug>

#include "widget/dialog/DIALOG.h"

namespace WIDGET ::MAIN {
void AppState::App_min() {
    qInfo() << "min by code";
    /// 设置对外层的widget缩小
    /// 这个设置属性似乎只能恢复一次？
    // Background::Get()->setWindowState(Qt::WindowState::WindowMinimized);
    Background::Get()->showMinimized();
}

void AppState::App_close() {
    auto dialog = DIALOG::Create_DIsVerify("是否确认关闭");
    QObject::connect(dialog, &DIALOG::DIsVerify::signal_yes, []() {
        qInfo() << "close by code";
        sLog_test() << "close by code";
        QApplication::quit();
    });
}

}  // namespace WIDGET::MAIN

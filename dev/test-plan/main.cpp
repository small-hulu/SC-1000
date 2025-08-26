#include <QApplication>
#include <QMessageBox>

#include "communication/COM.hpp"
#include "log/LOG.h"
#include "mainwindow.h"
#include "util/project_help.h"

void init_log() {
    qInstallMessageHandler(LOG::simple_messageHandler);

    QString filePath =
        "log/slog/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".log";
    LOG::SLog::Init_file(filePath);
    LOG::SLog::Init_console(true);
}

void init_com() {
    const bool ok = COM::Init();
    if (ok == false) {
        // qFatal(">>> 通信连接失败");
        qCritical(">>> 通信连接 fail");
        auto message = new QMessageBox(QMessageBox::Warning, "异常报警", "通信连接失败");
        message->show();
    } else {
        help_stop_reagentBin_loop();
        qDebug(">>> 通信连接 success");
    }
    // exit(0);
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    init_log();
    init_com();

    MainWindow w;
    w.show();
    w.setWindowTitle(qAppName() + qApp->applicationVersion());

    return app.exec();
}

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

#include "communication/COM_Device.hpp"
#include "communication/Version.h"
#include "communication/controller/Init.h"
#include "communication/singlestep/WSingleStepMain.h"
#include "log/LOG.h"

namespace {
QString init_log();

void unit_test() {
    using namespace COM;
}
}  // namespace

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    srand(time(0));

    qInstallMessageHandler(LOG::simple_messageHandler);
    QString filePath =
        "log/slog/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".log";
    LOG::SLog::Init_file(filePath);
    LOG::SLog::Init_console(true);

    qInfo() << "Main Thread:" << QThread::currentThread();
    qInfo().noquote().nospace() << init_log();
    qInfo().noquote().nospace() << LOG::message();
    qInfo().noquote().nospace() << COM::message();

    COM::Init();

    auto widget = new COM::SINGLEWSTEP::SingleStepMain{};
    widget->show();
    widget->setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);

    unit_test();

    int ret = app.exec();
    qInfo() << "App ExecOut =" << ret;
    return ret;
}

namespace {
QString init_log() {
    auto q_to_string = [](auto&& obj) -> QString {
        QString msg;
        QDebug  qos(&msg);
        qos << obj;
        return msg;
    };

    QJsonObject json;
    json["QSysInfo::prettyProductName()"]      = QSysInfo::prettyProductName();
    json["QSysInfo::productVersion()"]         = QSysInfo::productVersion();
    json["QSysInfo::currentCpuArchitecture()"] = QSysInfo::currentCpuArchitecture();
    json["qApp->applicationName()"]            = qApp->applicationName();
    json["qApp->applicationVersion()"]         = qApp->applicationVersion();
    json["qApp->desktop()->size()"]            = q_to_string(qApp->desktop()->size());
    json["__DATE__"]                           = __DATE__;
    json["__TIME__"]                           = __TIME__;
    json["__cplusplus"]                        = (int)__cplusplus;
    json["QT_VERSION_STR"]                     = QT_VERSION_STR;
    json["sizeof(void*) * 8"]                  = (int)sizeof(void*) * 8;
    json["QDateTime::currentDateTime()"] =
        QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss");
    json["QThread::currentThread()"] = q_to_string(QThread::currentThread());

    json["$Mode"] = QString("%1")
#ifdef QT_DEBUG
                        .arg("Debug");
#else
                        .arg("Release");
#endif

#ifdef __VERSION__
    json["__VERSION__"] = __VERSION__;
#endif

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}
}  // namespace

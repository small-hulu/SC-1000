#include "AppInit.h"

#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QDesktopWidget>
#include <QFont>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPrinter>
#include <QThread>

#include "AppEvent.h"
#include "CONTROLLER_Calibration.h"
#include "CONTROLLER_Consumable.h"
#include "CONTROLLER_ExpCalibration.h"
#include "CONTROLLER_ExpSample.h"
#include "CONTROLLER_LoginUser.h"
#include "CONTROLLER_Quality.h"
#include "CONTROLLER_Reagent.h"
#include "CONTROLLER_Sample.h"
#include "CONTROLLER_SampleApply.h"
#include "IniSC1000.h"
#include "communication/Version.h"
#include "communication/controller/Init.h"
#include "controller/CONTROLLER_ExpQuality.h"
#include "database/DB.hpp"
#include "entity/ENTITY_Log.h"
#include "global/GLOBAL_Path.hpp"
#include "log/LOG.h"
#include "printer/PRINTER.hpp"
#include "qrcode/QRCODE.hpp"

namespace CONTROLLER {
bool AppInit::App_Init() {
    srand(time(0));

#ifdef QT_NO_DEBUG
    print_debug();
#endif

    bool flag = true;

    //! 最重要，数据库的初始化
    QString dbFolder = to_absoluteFilePath(Path_DBFolder);
    QDir().mkpath(dbFolder);
    flag &= DB::Init(dbFolder, Path_DBFile);
    flag &= init_log();
    flag &= init_font();
    flag &= init_instance();

    return flag;
}

bool AppInit::App_Exit() {
    COM::Exit();
    DB::Exit();
    return true;
}

void AppInit::print_debug() {
    qInfo().noquote().nospace() << init_message();
    qInfo().noquote().nospace() << LOG::message();
    qInfo().noquote().nospace() << COM::message();
    qInfo().noquote().nospace() << CONTROLLER::IniSC1000::instance().to_string();
}

QString AppInit::init_message() {
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

/**
 * 初始化自定义的日志系统
 * ==================
 * 注意：因为要存在数据库中
 * 因此要在 db-init 之后操作
 */
bool AppInit::init_log() {
    auto log_to_db = [](const LOG::SLog* log) -> void {
        using attr = ENTITY::Log::attr;
        ENTITY::Log entity;
        entity.table[attr::file]    = log->file;
        entity.table[attr::func]    = log->func;
        entity.table[attr::line]    = QString::number(log->line);
        entity.table[attr::time]    = log->time;
        entity.table[attr::type]    = entity.Get_logTypeString(log->type);
        entity.table[attr::message] = log->message;
        entity.table[attr::remark]  = "V" + qApp->applicationVersion();

        DB::SqlExe::Insert_oneLine(entity.cdbtable, entity.Get_allValueList());
    };

    QString filePath = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    filePath         = "log/slog/" + filePath + ".log";
    LOG::SLog::Init_file(filePath);
    LOG::SLog::Append_task(log_to_db);

#ifndef QT_NO_DEBUG
    LOG::SLog::Init_console(true);
#endif

    return true;
}

/**
 * 设置全局字体
 * - 设置`pingfang`字体
 */
bool AppInit::init_font() {
    /// 设置全局字体样式
    QFont globalFont;
    globalFont.setFamily("PingFang SC Medium");
    qApp->setFont(globalFont);
    return true;
}

/**
 * 单例的构造
 * 这里可以一定程度的规定顺序
 */
bool AppInit::init_instance() {
    auto&& ini = IniSC1000::instance();

    COM::Init_instance();

    AppEvent::instance();

    LoginUser::instance();

    Consumable::instance();
    Sample::instance();
    Reagent::instance();
    Quality::instance();
    ExpSample::instance();
    ExpCalibration::instance();
    ExpQuality::instance();
    SampleApply::instance();

    QRCODE::SerialQrcode::instance();

    QPrinter printer(QPrinter::PrinterMode::HighResolution);
    printer.setPrinterName(ini[KI::printer_name].toString());
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    PRINTER::Helper::Init_printerConfig(printer);

    return true;
}

}  // namespace CONTROLLER

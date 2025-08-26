#include "LOG.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QTextCodec>
#include <mutex>

#include "LOG_Config.hpp"

namespace LOG {
/// data
/// 软件版本
static QString appVersion;
/// 日志存储路径
static QString logPath;
/// 日志文件后缀名
static QString suffixName;
/// 日期时间格式
static QString timeFormat;
/// 日志类型的字符串
static QVector<QString> typeStrList;

/**
 * @brief init_Config
 * 初始化配置
 * enum QtMsgType {
 *  QtDebugMsg,
 *  QtWarningMsg,
 *  QtCriticalMsg,
 *  QtFatalMsg,
 *  QtInfoMsg,
 *  QtSystemMsg = QtCriticalMsg
 * };
 */
static void init_config() {
    /// V1.2.3.4
    appVersion = QCoreApplication::applicationVersion();
    if (appVersion.isEmpty()) {
        appVersion = "No Version";
    } else {
        appVersion = "V" + appVersion;
    }

    /// .log
    suffixName = QString(_FILE_SUFFIX_NAME_);
    /// hh:mm:ss
    timeFormat = QString(_TIME_FORMAT_);
    /// ./SMTPCR/log/yyyy年MM月dd日
    logPath =
        QDir(_SAVE_PATH_).absoluteFilePath(QDateTime::currentDateTime().toString(_DATA_FIRMAT_));
    /// 生成path
    QDir().mkpath(logPath);

    typeStrList << QString("Debug");
    typeStrList << QString("Warning");
    typeStrList << QString("Critical");
    typeStrList << QString("Fatal");
    typeStrList << QString("Info");
    typeStrList << QString("System");
}

/**
 * @brief file_messageHandler
 * @param type
 * @param context
 * @param msg
 * 重定向的回调函数
 * 样例：[V0.1.2.0] [18:08:52] [@File:..\LOG\main.cpp @Func:int qMain(int,
 * char**) @Line:16]
 */
static void file_messageHandler(QtMsgType type, const QMessageLogContext& context,
                                const QString& msg) {
    QString filePath = QDir(logPath).absoluteFilePath(typeStrList[type] + suffixName);
    QString time     = QDateTime::currentDateTime().toString(timeFormat);
    QString locate   = QString("@File:%1 @Func:%2 @Line:%3")
                         .arg(context.file)
                         .arg(context.function)
                         .arg(context.line);
    QString str =
        QString("[%1] [%2] [%3]\r\n%4\r\n").arg(appVersion).arg(time).arg(locate).arg(msg);

    static std::mutex           mtx;
    std::lock_guard<std::mutex> lock(mtx);

    QFile file(filePath);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream(&file) << str;
    file.close();
}

void QLog_Init() {
    init_config();
#ifdef QT_NO_DEBUG
    qInstallMessageHandler(file_messageHandler);
#endif
}

void simple_messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg) {
    static QString filePath =
        "log/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + "sc1000log.log";
    static QFile            file(filePath);
    static QVector<QString> typeStrList;

    if (file.isOpen() == false) {
        file.open(QIODevice::WriteOnly | QIODevice::Append);
    }
    if (typeStrList.isEmpty()) {
        typeStrList << QString("Debug");
        typeStrList << QString("Warning");
        typeStrList << QString("Critical");
        typeStrList << QString("Fatal");
        typeStrList << QString("Info");
        typeStrList << QString("System");
    }

    QString attributeInfo;
    attributeInfo +=
        QString("@Time[%1] ").arg(QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss"));
    attributeInfo += QString("@Type[%1] ").arg(typeStrList[type]);
    attributeInfo += QString("@File[%1] ").arg(QFileInfo(context.file).fileName());
    attributeInfo += QString("@Line[%1] ").arg(context.line);

    QString str;
    str += QString(attributeInfo + "\r\n");
    str += QString(msg + "\r\n");

    static std::mutex           mtx;
    std::lock_guard<std::mutex> lock(mtx);

    auto consoleSS = QTextStream(stdout);
    consoleSS.setCodec("System");
    consoleSS << (str);

    auto fileSS = QTextStream(&file);
    fileSS.setCodec("UTF-8");
    fileSS << (str);
}

}  // namespace LOG

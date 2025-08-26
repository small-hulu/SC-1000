#include "LOG_Version.h"

#include <QJsonDocument>
#include <QJsonObject>

namespace LOG {
QString message() {
    QJsonObject json;
    json["app-name"] = QString("log");
    json["version"]  = QJsonDocument::fromJson(message_version().toLocal8Bit()).object();
    json["build"]    = QJsonDocument::fromJson(message_build().toLocal8Bit()).object();

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString message_version() {
    QJsonObject json;
    json["COM_VERSION"] =
        QString("V%1.%2.%3").arg(LOG_VERSION_MAJOR).arg(LOG_VERSION_MINOR).arg(LOG_VERSION_PATCH);

    json["QT_VERSION_STR"] = QT_VERSION_STR;
#ifdef __VERSION__
    json["__VERSION__"] = __VERSION__;
#endif

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

QString message_build() {
    QJsonObject json;
    json["sizeof(void*) * 8"] = (int)sizeof(void*) * 8;
    json["__DATE__"]          = __DATE__;
    json["__TIME__"]          = __TIME__;
    json["$Mode"]             = QString("%1")
#ifdef QT_DEBUG
                        .arg("Debug");
#else
                        .arg("Release");
#endif

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}
}  // namespace LOG

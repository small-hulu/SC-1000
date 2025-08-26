#include "EInterface_IConfig.h"

#include "global/GLOBAL_Path.hpp"

namespace ENTITY {

IConfig::IConfig() {
    QString path = QDir(Path_ConfigFolder).filePath(Path_ConfigFile);
    QFile   file(path);

    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        qWarning() << QString("File[%1] exist[%2] open[%3]")
                          .arg(path)
                          .arg(file.exists())
                          .arg(file.isOpen());
        return;
    }
    m_ojson = QJsonDocument::fromJson(file.readAll()).object();
}

bool IConfig::Save() {
    QString path = QDir(Path_ConfigFolder).filePath(Path_ConfigFile);
    QFile   file(path);
    if (false == file.exists()) {
        qWarning() << ("file Path is not exists.") << file.error() << file.errorString();
        return false;
    }
    if (false == file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << ("failed to open file for writing.") << file.error() << file.errorString();
        return false;
    }

    QJsonDocument jsonDoc(m_ojson);
    return file.write(jsonDoc.toJson());
}

}  // namespace ENTITY

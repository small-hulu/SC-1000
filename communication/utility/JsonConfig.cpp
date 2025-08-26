#include "JsonConfig.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>

#include "../config/CPath.hpp"
#include "IniConfig.h"

namespace COM {
JsonConfig::JsonConfig() {
    auto&& ini = IniConfig::instance();
    m_path     = to_absoluteFilePath(Path_ComFolder);
    QDir().mkpath(m_path);
    m_path = QDir(m_path).filePath(ini[KI::json_fix].toString());

    Refresh();
}

bool JsonConfig::Refresh() {
    QFile file(m_path);
    if (false == file.exists()) {
        qWarning() << m_path << ("file not exist or open error") << file.error()
                   << file.errorString();
        assert(false && "file is not exists");
        return false;
    }
    if (false == file.open(QFile::ReadOnly)) {
        qWarning() << m_path << ("file is open error") << file.error() << file.errorString();
        assert(false && "file is open error");
        return false;
    }

    m_json = QJsonDocument::fromJson(file.readAll()).object();
    assert(m_json.isEmpty() == false && "file is empty");
    return true;
}

bool JsonConfig::Save(const QJsonObject& json) {
    if (json.empty() == false) {
        m_json = json;
    }
    QFile file(m_path);
    if (false == file.exists()) {
        qWarning() << ("file Path is not exists.") << file.error() << file.errorString();
        return false;
    }
    if (false == file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << ("failed to open file for writing.") << file.error() << file.errorString();
        return false;
    }

    QJsonDocument jsonDoc(m_json);
    return file.write(jsonDoc.toJson());
}
}  // namespace COM

#ifndef CPATH_H_1716877161
#define CPATH_H_1716877161

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

namespace COM {

const char* const Path_ComFolder = "./sc1000-data/modbus";

const char* const Path_config = "sc1000-config.ini";

inline QString to_absoluteFilePath(const QString& path) {
    return QFileInfo(QCoreApplication::applicationDirPath(), path).absoluteFilePath();
}

}  // namespace COM
#endif  // CPATH_H_1716877161

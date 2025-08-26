#include "UTILITY_File.h"

#include <windows.h>

#include <QDebug>
#include <QDir>

namespace UTILITY {

QString Get_USBDiskPath() {
    /// 获取当前系统的盘符
    QFileInfoList drivesList  = QDir::drives();
    int           drivesIndex = -1;
    for (int i = 0; i < drivesList.count(); ++i) {
        UINT    driverType = GetDriveType((WCHAR *)drivesList[i].filePath().utf16());
        QString msg;

        switch (driverType) {
        case 0: {
            msg = "驱动类型不能确定";
        } break;
        case 1: {
            msg = "根路径无效";
        } break;
        case 2: {
            drivesIndex = i;
            msg = "可移动驱动器：软盘驱动器，拇指驱动器或闪存卡读取器";
        } break;
        case 3: {
            msg = "固定驱动器：硬盘驱动器或闪存驱动器";
        } break;
        case 4: {
            msg = "远程（网络）驱动器";
        } break;
        case 5: {
            msg = "CD-ROM驱动器";
        } break;
        case 6: {
            msg = "RAM磁盘";
        } break;
        default: break;
        }

        qDebug() << drivesList[i] << msg;
    }

    if (drivesIndex >= 0) {
        return drivesList[drivesIndex].filePath();
    }

    return "";
}

bool Save_toCSV(const QString &path, const QList<QList<QString> > &content) {
    QDir().mkpath(QFileInfo(path).absolutePath());
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << QString("[%1] open error {%2}").arg(path).arg(file.errorString());
        return false;
    }

    QTextStream stream(&file);

    for (auto &&sList : content) {
        stream << sList.join(",") << "\n";
    }

    file.close();
    return true;
}

}  // namespace UTILITY

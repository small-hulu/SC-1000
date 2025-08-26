#ifndef FILE_H_1723704383
#define FILE_H_1723704383

#include <QList>
#include <QString>

namespace UTILITY {

QString Get_USBDiskPath();

bool Save_toCSV(const QString& path, const QList<QList<QString>>& content);

}  // namespace UTILITY
#endif  // FILE_H_1723704383

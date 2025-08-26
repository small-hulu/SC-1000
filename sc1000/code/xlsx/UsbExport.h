#ifndef USBEXPORT_H
#define USBEXPORT_H

#include <QList>
#include <QObject>
#include <QString>
#include <QVariant>

#include "xlsxdocument.h"  // 使用QXlsx库

class UsbExport {
public:
    struct ExportData {
        QString SequenceNumber;
        QString Number;
        QString ProjectName;
        QString Result;
        QString Unit;
        QString expData;
        QString timeStart;
        QString timeEnd;
        QString state;
    };

    explicit UsbExport();

    // 导出数据到指定路径
    bool exportToPath(const QList<ExportData> &dataList, const QString &outputPath);

private:
    bool writeToExcel(const QList<ExportData> &dataList, const QString &filePath);
};

#endif  // USBEXPORT_H

#include "UsbExport.h"

UsbExport::UsbExport() {}

bool UsbExport::exportToPath(const QList<ExportData> &dataList, const QString &outputPath) {
    QString fullPath = outputPath + "/exported_data.xlsx";
    return writeToExcel(dataList, fullPath);
}

bool UsbExport::writeToExcel(const QList<ExportData> &dataList, const QString &filePath) {
    QXlsx::Document xlsx;

    // 写入表头
    xlsx.write(1, 1, "ID");
    xlsx.write(1, 2, "样本ID");
    xlsx.write(1, 3, "项目名称");
    xlsx.write(1, 4, "结果");
    xlsx.write(1, 5, "单位");
    xlsx.write(1, 6, "发光值");
    xlsx.write(1, 7, "开始时间");
    xlsx.write(1, 8, "结束时间");
    xlsx.write(1, 9, "状态");

    // 写入数据
    int row = 2;  // 从第二行开始写入数据
    for (const ExportData &data : dataList) {
        xlsx.write(row, 1, data.SequenceNumber);
        xlsx.write(row, 2, data.Number);
        xlsx.write(row, 3, data.ProjectName);
        xlsx.write(row, 4, data.Result);
        xlsx.write(row, 5, data.Unit);
        xlsx.write(row, 6, data.expData);
        xlsx.write(row, 7, data.timeStart);
        xlsx.write(row, 8, data.timeEnd);
        xlsx.write(row, 9, data.state);
        ++row;
    }

    // 保存文件
    return xlsx.saveAs(filePath);
}

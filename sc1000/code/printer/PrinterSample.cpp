#include "PrinterSample.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QPainter>
#include <QPrintPreviewWidget>
#include <QPrinter>
#include <QVBoxLayout>

#include "PHelper.h"
#include "controller/CONTROLLER_Config.h"
#include "controller/CONTROLLER_Project.h"
#include "global/GLOBAL_KeyProject.h"
namespace OM = KeyProject::OtherMessage;
namespace PRINTER {
void PrinterSample::painter_draw(QPaintDevice *device, double res, const QJsonObject &json) {
    QPainter painter(device);
    int      width    = device->width();
    int      height   = device->height();
    double   res_sign = res;  // 分辨率因子

    QFont font("黑体", 19);
    painter.setFont(font);
    QPen pen;
    pen.setColor(QColor(Qt::black));
    pen.setWidth(5);
    painter.setPen(pen);
    // 获取 JSON 数据
    QJsonArray  printerList = json[KeySample::PrinterList].toArray();
    QJsonObject data        = printerList[0].toObject();

    QRectF titleRect(400 * res_sign, 100 * res_sign, width - 800 * res_sign, 300 * res_sign);
    painter.drawText(titleRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     data[KeySample::ProjectName].toString() + "样本结果");

    font.setPointSize(12);
    painter.setFont(font);

    QStringList headers      = {"序号",       "编号",     "项目名称", "结果",
                                "单位",       "发光值",   "试剂批号", "底物批号",
                                "校准品批号", "申请时间", "完成时间", "校准时间"};
    QList<int>  columnWidths = {250, 350, 450, 450,  400,  400,
                                500, 400, 700, 1000, 1000, 800};  // 指定每列的宽度
    int         headerHeight = 100 * res_sign;                    // 表头高度
    int         rowHeight    = 300 * res_sign;                    // 行高度
    int         y            = 400 + headerHeight;
    int         x            = 0;

    painter.drawLine(0, y - 50, width, y - 50);
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(QRect(x, y, columnWidths[i] * res_sign, headerHeight), Qt::AlignCenter,
                         headers[i]);
        x += columnWidths[i] * res_sign;
    }
    y += headerHeight;
    painter.drawLine(0, y + 50, width, y + 50);
    for (const QJsonValue &value : printerList) {
        QJsonObject obj = value.toObject();
        x               = 0;
        for (int i = 0; i < headers.size(); ++i) {
            QString key;
            switch (i) {
            case 0 : key = KeySample::SequenceNumber; break;
            case 1 : key = KeySample::Number; break;
            case 2 : key = KeySample::ProjectName; break;
            case 3 : key = KeySample::Result; break;
            case 4 : key = KeySample::Unit; break;
            case 5 : key = KeySample::Luminous; break;
            case 6 : key = KeySample::ReagentBatchNumber; break;
            case 7 : key = KeySample::SubstrateBatchNumber; break;
            case 8 : key = KeySample::CalibrationBatchNumber; break;
            case 9 : key = KeySample::ApplyTime; break;
            case 10: key = KeySample::FinishTime; break;
            case 11: key = KeySample::CalibrationTime; break;
            }
            QString text = obj[key].toString();
            painter.drawText(QRect(x, y, columnWidths[i] * res_sign, rowHeight), Qt::AlignCenter,
                             text);
            x += columnWidths[i] * res_sign;
        }
        y += rowHeight;
    }
}

// 详细结果，医院用
void PrinterSample::painter_draw_history(QPaintDevice *device, double res,
                                         const QJsonObject &json) {
    QPainter painter(device);
    double   res_sign = res;
    QPen     pen;
    pen.setColor(QColor(Qt::black));
    pen.setWidth(2);
    painter.setPen(pen);

    int width  = device->width();
    int height = device->height();
    double dpiScaleFactor = device->physicalDpiX() / device->logicalDpiX();

    res_sign *= dpiScaleFactor;
    QFont titleFont("黑体", 16, QFont::Black);
    painter.setFont(titleFont);
    QRectF titleRect(0, 10 * res_sign, width, 150 * res_sign);
    painter.drawText(titleRect, Qt::AlignCenter, "检验报告单");

    QFont font("黑体", 10);
    painter.setFont(font);
    int y          = 240 * res_sign;
    int lineHeight = 200 * res_sign;

    // 解析json
    //qDebug().noquote() << QJsonDocument(json).toJson(QJsonDocument::Indented);

    auto get_data_from_json = [](QJsonObject json) -> ProjectMap
    {
        ProjectMap res_map;

        if (!json.contains(KeySample_history::PrinterList_history) || !json[KeySample_history::PrinterList_history].isArray())
            return res_map;

        QJsonArray arr = json[KeySample_history::PrinterList_history].toArray();

        int index = 1;
        for (const QJsonValue &itemVal : arr) {
            if (!itemVal.isObject()) continue;

            QJsonObject obj = itemVal.toObject();

            ProjectKey key(index, obj.value(KeySample_history::patient_patientId).toString());
            ProjectData values;
            values
                << obj.value(KeySample_history::ProjectName).toString()
                << obj.value(KeySample_history::FinishTime).toString()
                << obj.value(KeySample_history::ProjectName_Ch).toString()
                << obj.value(KeySample_history::Reference_range).toString()
                << obj.value(KeySample_history::Result).toString()
                << obj.value(KeySample_history::Result_explanation).toString()
                << obj.value(KeySample_history::Unit).toString()
                << obj.value(KeySample_history::patient_age).toString()
                << obj.value(KeySample_history::patient_ageUnit).toString()
                << obj.value(KeySample_history::patient_name).toString()
                << obj.value(KeySample_history::patient_sex).toString()
                << obj.value(KeySample_history::patient_type).toString()
                << obj.value(KeySample_history::timeStart).toString();
            res_map.insert(key, values);
            ++index;
        }
        return res_map;
    };

    auto add_result_explanation = [&](const QMap<QString, QString> &explanation) -> void {

        const int indentX = 700 * res_sign;

        for (auto it = explanation.constBegin(); it != explanation.constEnd(); ++it) {
            QString projectName = it.key() + "：";
            painter.drawText(QRectF(50 * res_sign, y, indentX, lineHeight),
                             Qt::AlignLeft | Qt::AlignVCenter,
                             projectName);

            qreal textX = 50 * res_sign + indentX;
            qreal textWidth = width - textX - 50 * res_sign;
            painter.drawText(QRectF(textX, y+60 , textWidth, lineHeight),
                             Qt::AlignLeft | Qt::TextWordWrap,
                             it.value());
            y += lineHeight;
        }
    };




    QJsonArray  printerList_history = json[KeySample_history::PrinterList_history].toArray();
    QJsonObject data                = printerList_history[0].toObject();
    painter.drawText(QRectF(50 * res_sign, y, 600 * res_sign, lineHeight), Qt::AlignLeft,
                     "患者姓名:");
    painter.drawText(QRectF(500 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_name].toString());
    painter.drawText(QRectF(1000 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     "性别:");
    painter.drawText(QRectF(1300 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_sex].toString());
    painter.drawText(QRectF(2000 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     "年龄:");
    painter.drawText(QRectF(2300 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_age].toString());
    painter.drawText(QRectF(3000 * res_sign, y, 600 * res_sign, lineHeight), Qt::AlignLeft,
                     "患者编号:");
    painter.drawText(QRectF(3400 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_patientId].toString());

    y += 120 * res_sign;

    painter.drawText(QRectF(50 * res_sign, y, 600 * res_sign, lineHeight), Qt::AlignLeft,
                     "送检科室：");
    painter.drawText(QRectF(500 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_department].toString());
    painter.drawText(QRectF(1000 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     "住院号:");
    painter.drawText(QRectF(1300 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_bedId].toString());
    painter.drawText(QRectF(2000 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     "床号:");
    painter.drawText(QRectF(2300 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_bedId].toString());
    painter.drawText(QRectF(3000 * res_sign, y, 600 * res_sign, lineHeight), Qt::AlignLeft,
                     "样本类型:");
    painter.drawText(QRectF(3400 * res_sign, y, 300 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_type].toString());

    y += lineHeight + 30 * res_sign;
    painter.drawLine(50 * res_sign, y, width - 50 * res_sign, y);  // 顶部横线
    y += 10 * res_sign;

    QStringList headers = {"序号", "英文名称", "中文名称", "结果", "参考范围", "单位"};
    int colWidths[] = {200, 500, 800, 900, 1000, 1100};
    int x           = 50 * res_sign;

    for (int i = 0; i < headers.size(); i++) {
        painter.drawText(QRectF(x, y, colWidths[i] * res_sign, lineHeight), Qt::AlignCenter,
                         headers[i]);
        x += colWidths[i] * res_sign;
    }

    y += lineHeight + 10 * res_sign;
    painter.drawLine(50 * res_sign, y, width - 50 * res_sign, y);



    ProjectMap res_map;    //para json
    res_map = get_data_from_json(json);

//    for (auto it = res_map.constBegin(); it != res_map.constEnd(); ++it) {
//        const ProjectKey &key = it.key();
//        const ProjectData &values = it.value();

//        qDebug() << "Index:" << key.first << " Name:" << key.second;

//        for (int i = 0; i < values.size(); ++i) {
//            qDebug() << "  [" << i << "]" << values[i];
//        }
//    }

    QStringList testResultsEng, testResultsChn, testResults, referenceRanges, units;
    if (data[KeySample_history::ProjectName].toString() == "25-OH-VD") {
        /// 对VD项目三联结果做显示

        using namespace CONTROLLER;
        auto    fileCU = Create_ProjectsFolderController();
        QString path   = fileCU.Get_projectPath("25-OH-VD");
        QFile   file(path);

        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            qWarning() << QString("File[%1] exist[%2] open[%3]")
                              .arg(path)
                              .arg(file.exists())
                              .arg(file.isOpen());
            return;
        }

        QJsonObject json1 = QJsonDocument::fromJson(file.readAll()).object();
        if (json1.isEmpty()) {
            qWarning() << "JSON 文件解析失败";
            return;
        }
        QJsonObject   OtherMessage       = json1.value(OM::OtherMessage).toObject();
        QJsonObject   projectMessage     = OtherMessage.value(OM::ProjectMessage).toObject();
        const QString Result_Calculation = projectMessage.value(OM::Result_Calculation).toString();
        QStringList   dataList           = data[KeySample_history::Result].toString().split(";");
        if (Result_Calculation == OM::Triplet) {
            testResultsEng.append(data[KeySample_history::ProjectName].toString());
            testResultsChn.append(data[KeySample_history::ProjectName_Ch].toString());
            testResults.append(dataList[0]);
            referenceRanges.append(data[KeySample_history::Reference_range].toString());
            units.append(data[KeySample_history::Unit].toString());

            testResultsEng.append("25-OH-VD2");
            testResultsChn.append("25-羟基维生素D2");
            testResults.append(dataList[1]);
            referenceRanges.append("3.3--8.89");
            units.append(data[KeySample_history::Unit].toString());

            testResultsEng.append("25-OH-VD3");
            testResultsChn.append("25-羟基维生素D3");
            testResults.append(dataList[2]);
            referenceRanges.append("30.00--80.00");
            units.append(data[KeySample_history::Unit].toString());
        } else {
            testResultsEng.append(data[KeySample_history::ProjectName].toString());
            testResultsChn.append(data[KeySample_history::ProjectName_Ch].toString());
            testResults.append(dataList[0]);
            referenceRanges.append(data[KeySample_history::Reference_range].toString());
            units.append(data[KeySample_history::Unit].toString());
        }
    }
    else {
        testResultsEng.append(data[KeySample_history::ProjectName].toString());
        testResultsChn.append(data[KeySample_history::ProjectName_Ch].toString());
        testResults.append(data[KeySample_history::Result].toString());
        referenceRanges.append(data[KeySample_history::Reference_range].toString());
        units.append(data[KeySample_history::Unit].toString());
    }

    QMap<QString, QList<QStringList>> groupedData;
    QMap<QString,QString> explanation;
    // 先按名字分组
    for (auto it = res_map.constBegin(); it != res_map.constEnd(); ++it) {
        QString name = it.key().second; // ProjectName
        groupedData[name].append(it.value());
    }
    // 绘制
    for (auto it = groupedData.constBegin(); it != groupedData.constEnd(); ++it) {
        const QList<QStringList> &rows = it.value();
        for (int i = 0; i < rows.size(); ++i) {
            const QStringList &row = rows[i];
            explanation.insert(row[0], row[3]);
            x = 50 * res_sign;

            painter.drawText(QRectF(x, y, colWidths[0] * res_sign, lineHeight), Qt::AlignCenter,
                             QString::number(i + 1));
            x += colWidths[0] * res_sign;

            painter.drawText(QRectF(x, y, colWidths[1] * res_sign, lineHeight), Qt::AlignCenter,
                             row[0]); // 英文项目名
            x += colWidths[1] * res_sign;

            painter.drawText(QRectF(x, y, colWidths[2] * res_sign, lineHeight), Qt::AlignCenter,
                             row[2]); // ProjectName_Ch
            x += colWidths[2] * res_sign;

            painter.drawText(QRectF(x, y, colWidths[3] * res_sign, lineHeight), Qt::AlignCenter,
                             row[4]); // Result
            x += colWidths[3] * res_sign;

            painter.drawText(QRectF(x, y, colWidths[4] * res_sign, lineHeight), Qt::AlignCenter,
                             row[5]); // Reference_range
            x += colWidths[4] * res_sign;

            painter.drawText(QRectF(x, y, colWidths[5] * res_sign, lineHeight), Qt::AlignCenter,
                             row[6]); // Unit

            y += lineHeight - 100 * res_sign;
        }
    }

    y += 2 * lineHeight + 50 * res_sign;
    painter.drawText(QRectF(50 * res_sign, y, width - 100 * res_sign, 3 * lineHeight),
                     Qt::AlignLeft | Qt::TextWordWrap, "检测结果说明:");

    y += 120 * res_sign;
    add_result_explanation(explanation);

    y += (4 - explanation.size()) * lineHeight;

    y += lineHeight + 10 * res_sign;
    painter.drawLine(50 * res_sign, y, width - 50 * res_sign, y);  // 第三条横线

    painter.drawLine(50 * res_sign, y, width - 50 * res_sign, y);  // 底部横线
    y += 20 * res_sign;

    painter.drawText(QRectF(50 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     "送检医生:");
    painter.drawText(QRectF(500 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_inspection].toString());
    painter.drawText(QRectF(2000 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     "检验医生:");
    painter.drawText(QRectF(2400 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_verifier].toString());
    painter.drawText(QRectF(3500 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     "审核者:");
    painter.drawText(QRectF(3800 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::patient_auditor].toString());

    y += 120 * res_sign;
    painter.drawText(QRectF(50 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     "检验时间:");
    painter.drawText(QRectF(500 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     data[KeySample_history::timeStart].toString());
    painter.drawText(QRectF(2000 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     "报告时间:");
    QDateTime currentTime = QDateTime::currentDateTime();

    painter.drawText(QRectF(2500 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     currentTime.toString("yyyy-MM-dd hh:mm:ss"));

    y += lineHeight;
    painter.drawText(QRectF(50 * res_sign, y, width - 100 * res_sign, lineHeight), Qt::AlignLeft,
                     "声明：本报告单仅对本标本负责。");
}

// 热敏打印
void PrinterSample::painter_draw_ts(QPaintDevice *device, double res, const QJsonObject &json) {
    QPainter painter(device);

    double res_sign = res;
    QPen   pen;
    pen.setColor(QColor(Qt::black));
    pen.setWidth(2);
    painter.setPen(pen);

    int    width          = device->width();
    int    height         = device->height();
    double dpiScaleFactor = device->physicalDpiX() / device->logicalDpiX();

    res_sign                   *= dpiScaleFactor;
    // 解析json
    QJsonArray  printerList_ts  = json[KeySample_ts::PrinterList_ts].toArray();
    QJsonObject data            = printerList_ts[0].toObject();

    // 绘制标题
    QFont titleFont("黑体", 10);
    painter.setFont(titleFont);
    QRectF                       titleRect(0, 20 * res_sign, width, 200 * res_sign);
    CONTROLLER::Config::Hospital hospital;
    painter.drawText(titleRect, Qt::AlignHCenter | Qt::AlignVCenter,
                     hospital.hospitalName + "\n检验报告单");
    QFont font("黑体", 8);
    painter.setFont(font);

    painter.drawLine(0, 210 * res_sign, width, 210 * res_sign);
    // 对VD项目的打印处理
    QStringList labels;
    if (data[KeySample_ts::ProjectName].toString() == "25-OH-VD") {
        using namespace CONTROLLER;
        auto    fileCU = Create_ProjectsFolderController();
        QString path   = fileCU.Get_projectPath("25-OH-VD");
        QFile   file(path);

        if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
            qWarning() << QString("File[%1] exist[%2] open[%3]")
                              .arg(path)
                              .arg(file.exists())
                              .arg(file.isOpen());
            return;
        }

        QJsonObject json1 = QJsonDocument::fromJson(file.readAll()).object();
        if (json1.isEmpty()) {
            qWarning() << "JSON 文件解析失败";
            return;
        }
        QJsonObject   OtherMessage       = json1.value(OM::OtherMessage).toObject();
        QJsonObject   projectMessage     = OtherMessage.value(OM::ProjectMessage).toObject();
        const QString Result_Calculation = projectMessage.value(OM::Result_Calculation).toString();
        QStringList   dataList           = data[KeySample_ts::Result].toString().split(";");
        if (Result_Calculation == OM::Triplet) {
            labels.append("患者编号：" + data[KeySample_ts::patient_num].toString());
            labels.append("患者姓名：" + data[KeySample_ts::patient_name].toString());
            labels.append("患者年龄：" + data[KeySample_ts::patient_age].toString());
            labels.append("患者性别：" + data[KeySample_ts::patient_sex].toString());
            labels.append("检测项目：" + data[KeySample_ts::ProjectName].toString());
            labels.append("检测批号：" + data[KeySample_ts::ReagentBatchNumber].toString());
            labels.append("检测结果:");
            labels.append(data[KeySample_ts::ProjectName].toString() + ":" + dataList[0] +
                          projectMessage.value(OM::Default_Unit).toString());
            labels.append("25-OH-VD2:" + dataList[1] +
                          projectMessage.value(OM::Default_Unit).toString());
            labels.append("25-OH-VD3:" + dataList[2]);
            labels.append("参考范围:");
            labels.append(data[KeySample_ts::ProjectName].toString() + ":" +
                          data[KeySample_ts::Reference_range].toString());
            labels.append("检测日期:" + data[KeySample_ts::FinishTime].toString());
        } else {
            labels.append("患者编号：" + data[KeySample_ts::patient_num].toString());
            labels.append("患者姓名：" + data[KeySample_ts::patient_name].toString());
            labels.append("患者年龄：" + data[KeySample_ts::patient_age].toString());
            labels.append("患者性别：" + data[KeySample_ts::patient_sex].toString());
            labels.append("检测项目：" + data[KeySample_ts::ProjectName].toString());
            labels.append("检测批号：" + data[KeySample_ts::ReagentBatchNumber].toString());
            labels.append("检测结果:");
            labels.append(data[KeySample_ts::ProjectName].toString() + ":" + dataList[0]);
            labels.append("参考范围:");
            labels.append(data[KeySample_ts::ProjectName].toString() + ":" +
                          data[KeySample_ts::Reference_range].toString());
            labels.append("检测日期:" + data[KeySample_ts::FinishTime].toString());
        }
    } else {
        labels.append("患者编号：" + data[KeySample_ts::patient_num].toString());
        labels.append("患者姓名：" + data[KeySample_ts::patient_name].toString());
        labels.append("患者年龄：" + data[KeySample_ts::patient_age].toString());
        labels.append("患者性别：" + data[KeySample_ts::patient_sex].toString());
        labels.append("检测项目：" + data[KeySample_ts::ProjectName].toString());
        labels.append("检测批号：" + data[KeySample_ts::ReagentBatchNumber].toString());
        labels.append("检测结果:");
        labels.append(data[KeySample_ts::ProjectName].toString() + ":" +
                      data[KeySample_ts::Result].toString());
        labels.append("参考范围:");
        labels.append(data[KeySample_ts::ProjectName].toString() + ":" +
                      data[KeySample_ts::Reference_range].toString());
        labels.append("检测日期:" + data[KeySample_ts::FinishTime].toString());
    }

    int y = 230 * res_sign;

    int lineHeight  = 70 * res_sign;
    int lineSpacing = 35 * res_sign;

    for (const QString &label : labels) {
        QRectF labelRect(50 * res_sign, y, width - 100 * res_sign, lineHeight);
        painter.drawText(labelRect, Qt::AlignLeft | Qt::AlignVCenter, label);
        y += lineHeight + lineSpacing;  // 增加行间距
    }

    painter.drawLine(0, y + 10 * res_sign, width, y + 10 * res_sign);

    QRectF noteRect(0, y + 20 * res_sign, width, 80 * res_sign);
    painter.drawText(noteRect, Qt::AlignCenter, "注：本报告只对此次检测有效。");
}

bool PrinterSample::Exec_print(const QJsonObject &json) {
    static QPrinter printer(QPrinter::HighResolution);
    // Helper::Reset_QPrinter(printer);

    // 获取打印机配置
    ENTITY::Printer         printerConfig;
    ENTITY::PrinterSettings settings = printerConfig.LoadFromJson();
    printer.setPrinterName(settings.printerName);

    if (settings.printmode == ST::Thermal_Report)  // 热敏
    {
        printer.setPageSize(QPrinter::Custom);
        printer.setOrientation(QPrinter::Portrait);
        printer.setPaperSize(QSizeF(55.0, 100.0), QPrinter::Millimeter);  // 55mm宽，100mm高
        PrinterSample::painter_draw_ts(&printer, settings.res.toDouble(), json);  // A4 A5
    } else if (settings.printmode == ST::Inspection_Report) {
        printer.setPageSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);
        PrinterSample::painter_draw_history(&printer, settings.res.toDouble(), json);  // A4 A5
    } else if (settings.printmode == ST::Sample_Report) {
        printer.setPageSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Landscape);
        PrinterSample::painter_draw(&printer, settings.res.toDouble(), json);  // A4 A5
    }

    return true;
}

QPrintPreviewWidget *PrinterSample::Create_widget(const QJsonObject &json, double res,
                                                  QWidget *parent) {
    static QPrinter         printer(QPrinter::HighResolution);
    ENTITY::Printer         printerConfig;
    ENTITY::PrinterSettings settings = printerConfig.LoadFromJson();
    printer.setPrinterName(settings.printerName);

    QPrintPreviewWidget *previewWidget = nullptr;

    if (settings.printmode == ST::Thermal_Report) {
        printer.setPageSize(QPrinter::Custom);
        printer.setOrientation(QPrinter::Portrait);
        printer.setPaperSize(QSizeF(55.0, 100.0), QPrinter::Millimeter);  // 55mm宽，100mm高
        previewWidget = new QPrintPreviewWidget(&printer, parent);        // 初始化预览部件

        QObject::connect(
            previewWidget, &QPrintPreviewWidget::paintRequested,
            [json, res](QPrinter *printer) { PrinterSample::painter_draw_ts(printer, res, json); });

    } else if (settings.printmode == ST::Inspection_Report) {
        printer.setPageSize(QPrinter::A5);
        printer.setOrientation(QPrinter::Landscape);
        previewWidget = new QPrintPreviewWidget(&printer, parent);  // 初始化预览部件

        QObject::connect(previewWidget, &QPrintPreviewWidget::paintRequested,
                         [json, res](QPrinter *printer) {
                             PrinterSample::painter_draw_history(printer, res, json);
                         });

    } else if (settings.printmode == ST::Sample_Report) {
        printer.setPageSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Landscape);
        previewWidget = new QPrintPreviewWidget(&printer, parent);  // 初始化预览部件

        QObject::connect(
            previewWidget, &QPrintPreviewWidget::paintRequested,
            [json, res](QPrinter *printer) { PrinterSample::painter_draw(printer, res, json); });
    }

    // 最后返回 QPrintPreviewWidget 对象
    return previewWidget;
}
QWidget *PrinterSample::Create_widget_setting_preview(QString type, double res,
                                                      const QJsonObject &json, QWidget *parent) {
    static QPrinter         printer(QPrinter::HighResolution);
    ENTITY::Printer         printerConfig;
    ENTITY::PrinterSettings settings = printerConfig.LoadFromJson();

    if (type == ST::Thermal_Report) {
        printer.setPageSize(QPrinter::Custom);
        printer.setOrientation(QPrinter::Portrait);
        printer.setPaperSize(QSizeF(55.0, 100.0), QPrinter::Millimeter);  // 55mm宽，100mm高
        QWidget             *contentWidget = new QWidget(parent);
        QPrintPreviewWidget *previewWidget = new QPrintPreviewWidget(&printer, contentWidget);
        QVBoxLayout         *layout        = new QVBoxLayout(contentWidget);
        contentWidget->setLayout(layout);

        QObject::connect(
            previewWidget, &QPrintPreviewWidget::paintRequested,
            [json, res](QPrinter *printer) { PrinterSample::painter_draw_ts(printer, res, json); });

        layout->addWidget(previewWidget);
        return contentWidget;
    } else if (type == ST::Inspection_Report) {
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A5);
        QWidget             *contentWidget = new QWidget(parent);
        QPrintPreviewWidget *previewWidget = new QPrintPreviewWidget(&printer, contentWidget);
        QVBoxLayout         *layout        = new QVBoxLayout(contentWidget);
        contentWidget->setLayout(layout);

        QObject::connect(previewWidget, &QPrintPreviewWidget::paintRequested,
                         [json, res](QPrinter *printer) {
                             PrinterSample::painter_draw_history(printer, res, json);
                         });

        layout->addWidget(previewWidget);
        return contentWidget;
    } else if (type == ST::Sample_Report) {
        printer.setOrientation(QPrinter::Landscape);
        printer.setPageSize(QPrinter::A4);
        QWidget             *contentWidget = new QWidget(parent);
        QPrintPreviewWidget *previewWidget = new QPrintPreviewWidget(&printer, contentWidget);
        QVBoxLayout         *layout        = new QVBoxLayout(contentWidget);
        contentWidget->setLayout(layout);

        QObject::connect(
            previewWidget, &QPrintPreviewWidget::paintRequested,
            [json, res](QPrinter *printer) { PrinterSample::painter_draw(printer, res, json); });

        layout->addWidget(previewWidget);
        return contentWidget;
    }
}

void PrinterSample::Exec_print_signal(QJsonObject json) {
    Exec_print(json);
}

}  // namespace PRINTER

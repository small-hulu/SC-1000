#include "PHelper.h"

#include <QDebug>
#include <QPainter>

namespace PRINTER {

namespace {
/**
 * 配置缓存
 * 由于要在 ACoreApplication 之后创建
 * 因此使用 local-static 的方式
 */
QPrinter& Get_QPrinter_globalConfig_instance() {
    static QPrinter g_printer(QPrinter::PrinterMode::HighResolution);
    return g_printer;
}
}  // namespace

void Helper::Init_printerConfig(QPrinter& printer) {
    QPrinter& target = Get_QPrinter_globalConfig_instance();
    QPrinter& origin = printer;
    target.setPrinterName(origin.printerName());
    target.setPageSize(origin.pageSize());
    target.setOrientation(origin.orientation());
}

/**
 * 从全局配置中获得打印机的名称
 */
bool Helper::Reset_QPrinter(QPrinter& printer) {
    QPrinter& target = printer;
    QPrinter& origin = Get_QPrinter_globalConfig_instance();
    target.setPrinterName(origin.printerName());
    target.setPageSize(origin.pageSize());
    target.setOrientation(origin.orientation());

    return printer.isValid();
}

void Helper::Print_byWidget(QWidget* widget) {
    assert(widget);

    QPrinter printer(QPrinter::HighResolution);
    Reset_QPrinter(printer);

    if (printer.isValid()) {
        QPainter painter(&printer);
        painter.save();
        painter.setWindow(widget->rect());
        widget->render(&painter);
        painter.restore();
    } else {
        qWarning() << "打印机异常" << printer.printerState();
    }
}

}  // namespace PRINTER

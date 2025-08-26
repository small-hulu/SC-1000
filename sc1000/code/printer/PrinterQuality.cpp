#include "PrinterQuality.h"

namespace PRINTER {

void PrinterQuality::painter_draw(QPaintDevice *device, const QJsonObject &json) {}

bool PrinterQuality::Exec_print(const QJsonObject &json) {
    return true;
}

QWidget *PrinterQuality::Create_widget(const QJsonObject &json, QWidget *parent) {
    return nullptr;
}

}  // namespace PRINTER

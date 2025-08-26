#include "PrinterCalibration.h"

namespace PRINTER {

void PrinterCalibration::painter_draw(QPaintDevice *device, const QJsonObject &json) {}

bool PrinterCalibration::Exec_print(const QJsonObject &json) {
    return true;
}

QWidget *PrinterCalibration::Create_widget(const QJsonObject &json, QWidget *parent) {
    return nullptr;
}

}  // namespace PRINTER

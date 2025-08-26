#include "PrinterReagent.h"

namespace PRINTER {

void PrinterReagent::painter_draw(QPaintDevice *device, const QJsonObject &json) {}

bool PrinterReagent::Exec_print(const QJsonObject &json) {
    return true;
}

QWidget *PrinterReagent::Create_widget(const QJsonObject &json, QWidget *parent) {
    return nullptr;
}

}  // namespace PRINTER

#include "EConfig_Printer.h"

namespace ENTITY {

void Printer::setPrint_mode(const QString &value) {
    print_mode = value;
}

QString Printer::getPrint_mode() const {
    return print_mode;
}

QString Printer::getPreview_mode() const {
    return preview_mode;
}

void Printer::setPreview_mode(const QString &value) {
    preview_mode = value;
}

Printer::Printer() {
    namespace Key = KeyPrinter;
    json          = m_ojson[Key::Printer].toObject();
    printer.setPrinterName(json[Key::PrinterName].toString());
    setPrint_mode(json[Key::printmode].toString());
    setPreview_mode(json[Key::preview_mode].toString());
    setRes(json[Key::res].toString());
}

bool Printer::Save() {
    namespace Key           = KeyPrinter;
    json[Key::PrinterName]  = printer.printerName();
    json[Key::printmode]    = print_mode;
    json[Key::preview_mode] = preview_mode;
    json[Key::res]          = res;
    m_ojson[Key::Printer]   = json;
    return IConfig::Save();
}

QString Printer::getRes() const {
    return res;
}

void Printer::setRes(const QString &value) {
    res = value;
}
// 从 JSON 文件中加载打印机配置信息
PrinterSettings Printer::LoadFromJson() {
    namespace Key = KeyPrinter;
    json          = m_ojson[Key::Printer].toObject();

    PrinterSettings settings;

    QString printerName = json[Key::PrinterName].toString();
    if (!printerName.isEmpty()) {
        settings.printerName = printerName;
        printer.setPrinterName(printerName);
    }
    QString printmode     = json[Key::printmode].toString();
    QString preview_mode  = json[Key::preview_mode].toString();
    QString res           = json[Key::res].toString();
    settings.printmode    = printmode;
    settings.preview_mode = preview_mode;
    settings.res          = res;
    return settings;
}
}  // namespace ENTITY

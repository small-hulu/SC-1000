#ifndef PRINTER_H_1725689624
#define PRINTER_H_1725689624

#include <QPrinter>

#include "EInterface_IConfig.h"

namespace ENTITY {

namespace KeyPrinter {
const QString Printer      = "Printer";
const QString PrinterName  = "PrinterName";
const QString printmode    = "printmode";
const QString preview_mode = "preview_mode";
const QString res          = "res";
}  // namespace KeyPrinter

struct PrinterSettings {
    QString printerName;
    QString printmode;
    QString preview_mode;
    QString res;
};

class Printer : public IConfig {
public:
    QPrinter printer;

public:
    Printer();
    virtual bool Save() override;

private:
    QString print_mode;
    QString preview_mode;
    QString res;

public:
    // 得到打印配置
    PrinterSettings LoadFromJson();
    void            setPrint_mode(const QString &value);
    QString         getPrint_mode() const;
    QString         getPreview_mode() const;
    void            setPreview_mode(const QString &value);
    QString         getRes() const;
    void            setRes(const QString &value);
};
}  // namespace ENTITY
#endif  // PRINTER_H

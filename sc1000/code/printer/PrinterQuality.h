#ifndef PQUALITY_H_1724470327
#define PQUALITY_H_1724470327

#include <QJsonObject>
#include <QPaintDevice>

namespace PRINTER {
namespace KeyQuality {
const QString PrinterList = "PrinterList";
}  // namespace KeyQuality

class PrinterQuality {
private:
    static void painter_draw(QPaintDevice *device, const QJsonObject &json);

public:
    static bool     Exec_print(const QJsonObject &json);
    static QWidget *Create_widget(const QJsonObject &json, QWidget *parent = nullptr);
};
}  // namespace PRINTER
#endif  // PQUALITY_H_1724470327

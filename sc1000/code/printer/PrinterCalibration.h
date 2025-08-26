#ifndef PRINTCALIBRATION_H_1724470521
#define PRINTCALIBRATION_H_1724470521

#include <QJsonObject>
#include <QWidget>

namespace PRINTER {
namespace KeyCalibration {
const QString PrinterList = "PrinterList";
}  // namespace KeyCalibration

class PrinterCalibration {
private:
    static void painter_draw(QPaintDevice *device, const QJsonObject &json);

public:
    static bool     Exec_print(const QJsonObject &json);
    static QWidget *Create_widget(const QJsonObject &json, QWidget *parent = nullptr);
};
}  // namespace PRINTER

#endif  // PRINTCALIBRATION_H_1724470521

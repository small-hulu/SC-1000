#ifndef PRINTERREAGENT_H_1724470628
#define PRINTERREAGENT_H_1724470628

#include <QJsonObject>
#include <QWidget>

namespace PRINTER {
namespace KeyReagent {
const QString PrinterList = "PrinterList";
}  // namespace KeyReagent

class PrinterReagent {
private:
    static void painter_draw(QPaintDevice *device, const QJsonObject &json);

public:
    static bool     Exec_print(const QJsonObject &json);
    static QWidget *Create_widget(const QJsonObject &json, QWidget *parent = nullptr);
};

}  // namespace PRINTER
#endif  // PRINTERREAGENT_H_1724470628

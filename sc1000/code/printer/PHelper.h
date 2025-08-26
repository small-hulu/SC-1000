#ifndef PRINTER_H_1722823366
#define PRINTER_H_1722823366

#include <QPrinter>
#include <QWidget>

namespace PRINTER {

struct Helper {
    /**
     * 设置全局状态
     */
    static void Init_printerConfig(QPrinter& printer);

    /**
     * 初始化默认打印参数
     * 主要是设置打印机的名字
     * ================
     * 默认纵向打印
     */
    static bool Reset_QPrinter(QPrinter& printer);

    /**
     * 直接打印widget
     * =============
     * 目前看来效果不是很好
     */
    static void Print_byWidget(QWidget* widget);
};

}  // namespace PRINTER

#endif  // PRINTER_H_1722823366

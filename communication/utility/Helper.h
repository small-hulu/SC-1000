#ifndef HELPER_H_1724749889
#define HELPER_H_1724749889

#include <QJsonObject>
#include <QList>
#include <QString>

#include "../COM_Device.hpp"
#include "../config/CKeyJson.hpp"
#include "communication/bootloader/bootloader_operation.h"

template <class T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
    assert(!(hi < lo));
    return v < lo ? lo : hi < v ? hi : v;
}

namespace COM {
struct Helper {
    template <typename Device>
    static bool can_check(Device&& device) {
        return can_check(device.get_dataUnitConfig().deviceAddress);
    }

    static bool can_check(int deviceAddress) {
        auto& modbus = ModbusImpl::instance();
        if (modbus.isOpen() == false) {
            return false;
        }
        BOOTLOADER::BootloaderOp boot(deviceAddress);
        return (boot.check_is_bootloader() != 1);
    }

    /**
     * 周期性就近原则
     * @param curLocal      当前位置
     * @param targetLocal   目标位置
     * @param cycle         一圈的长度
     */
    static int offset_byProximity(int curLocal, int targetLocal, int cycle);

    /** 根据板孔表设置"二维数组" 8x7
     * ===========================================
     *  <x-----------------------------------------
     * |  c(6,0)
     * |    [6]   [5]   [4]   [3]   [2]   [1]   [0]
     * | [0] o     o     o     o     o     o     o   a(0,0)
     * | [1] o     o     o     o     o     o     o
     * | [2] o     o     o     o     o     o     o
     * | [3] o     o     o     o     o     o     o
     * | [4] o     o     o     o     o     o     o
     * | [5] o     o     o     o     o     o     o
     * y [6] o     o     o     o     o     o     o   b(0,6)
     * v [7]                         o     o     o
     * ===========================================
     */
    struct CalcHoles {
        struct Point {
            int i = -1;
            int j = -1;
            int x = 0;
            int y = 0;

            QString to_string() const {
                return QString::asprintf(R"({"i":%d, "j":%d, "x":%d, "y":%d})", i, j, x, y);
            }

            QJsonObject to_json() const {
                QJsonObject json;
                json[KJ::i] = i;
                json[KJ::j] = j;
                json[KJ::x] = x;
                json[KJ::y] = y;
                return json;
            }
        };


        Point a;
        Point b;
        Point c;

        CalcHoles() {
            a.i = 0;
            a.j = 0;
            b.i = 0;
            b.j = 6;
            c.i = 6;
            c.j = 0;
        }

        Point convert(int tarAxisI, int tarAxisJ) {
            int x_axis_up_delta_x = (c.x - a.x) / (6 - 0);
            int x_axis_up_delta_y = (c.y - a.y) / (6 - 0);

            int y_axis_up_delta_x = (b.x - a.x) / (6 - 0);
            int y_axis_up_delta_y = (b.y - a.y) / (6 - 0);

            Point p;
            p.i = tarAxisI;
            p.j = tarAxisJ;
            p.x = a.x + x_axis_up_delta_x * (tarAxisI - 0) + y_axis_up_delta_x * (tarAxisJ - 0);
            p.y = a.y + y_axis_up_delta_y * (tarAxisJ - 0) + x_axis_up_delta_y * (tarAxisI - 0);
            return p;
        }

        QList<Point> get_pointList() {
            std::vector<std::pair<int, int>> pointers;
            for (int i = 0; i <= 2; i += 1) {
                for (int j = 0; j <= 7; j += 1) {
                    pointers.emplace_back(i, j);
                }
            }
            for (int i = 3; i <= 6; i += 1) {
                for (int j = 0; j <= 6; j += 1) {
                    pointers.emplace_back(i, j);
                }
            }

            QList<Point> pList;
            for (auto [i, j] : pointers) {
                pList << convert(i, j);
            }
            return pList;
        }
    };
};
}  // namespace COM

#endif  // HELPER_H_1724749889

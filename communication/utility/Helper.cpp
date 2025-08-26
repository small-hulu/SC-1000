#include "Helper.h"

#include <QDebug>
#include <cmath>

namespace COM {

int Helper::offset_byProximity(int curLocal, int targetLocal, int cycle) {
    const int oval    = curLocal;
    auto      to_plus = [cycle = abs(cycle)](int val) {
        while (val < 0) {
            val += cycle;
        }
        val %= cycle;
        return val;
    };

    /// 所有数值，均转换为正数
    cycle       = abs(cycle);
    targetLocal = to_plus(targetLocal);
    curLocal    = to_plus(curLocal);

    if (targetLocal >= curLocal) {
        int len1 = targetLocal - curLocal;
        int len2 = cycle - len1;
        if (len1 <= len2) {
            return oval + len1;
        } else {
            return oval - len2;
        }
    } else {
        int len1 = curLocal - targetLocal;
        int len2 = cycle - len1;
        if (len1 <= len2) {
            return oval - len1;
        } else {
            return oval + len2;
        }
    }
}

}  // namespace COM

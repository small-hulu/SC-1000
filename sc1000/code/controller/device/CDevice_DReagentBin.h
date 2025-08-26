#ifndef REAGENTBIN_H_1717728707
#define REAGENTBIN_H_1717728707

#include "communication/utility/IniConfig.h"

namespace CONTROLLER::CDevice {
/**
 * 试剂仓
 * 操作试剂仓的对应的位置转到最前方
 */
struct DReagentBin {
    //! 目前定死10个位置
    //! ui层面的范围是[1, n]
    //! 但是控制逻辑中是[0, n - 1]
    static constexpr int BinCount = 10;

    /// 试剂船位置
    static bool ToFront_forReagent(int idx);
    /// 样本位置
    static bool ToFront_forSample(int idx);

    /// 是否自动混匀
    static void Switch_spinLoop(bool open);
};
}  // namespace CONTROLLER::CDevice
#endif  // REAGENTBIN_H_1717728707

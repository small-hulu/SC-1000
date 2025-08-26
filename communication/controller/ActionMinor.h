#ifndef CACTIONMINOR_H_1713408992
#define CACTIONMINOR_H_1713408992

#include "../entity/TaskNode.h"

namespace COM {
class ActionMinor {
public:
    static TaskNode Gen_TaskType(const QJsonObject& json);

public:
    /**
     * 夹爪
     * = 从from夹取 ->
     *   到to释放
     *  ================================
     *   {
     *       "actionType": "ActionMinor",
     *       "deviceName": "Gripper",
     *       "from": {
     *           "target": "Cup"
     *       },
     *       "to": {
     *           "target": "Load"
     *       }
     *   }
     */
    static TaskNode Minor_for_Gripper_from_to(const QJsonObject& json);

    /**
     * 移液针
     * = 从from取液 ->
     *   到to排液
     * ======================================
     *   {
     *       "actionType": "ActionMinor",
     *       "deviceName": "PipettingNeedle",
     *       "from": {
     *           "target": "ReagentBin",
     *           "idxBinBack": 6,
     *           "idxBinGroup": 0
     *       },
     *       "to": {
     *           "target": "Load"
     *       }
     *   }
     */
    static TaskNode Minor_for_PipetteNeedle_from_to(const QJsonObject& json);

    /**
     * 同时配多组，这里配两组，中间使用清洗外壁模式
     * {
     *    "actionType": "ActionMinor",
     *    "deviceName": "DoubleSuction",
     *    "from": [
     *        {
     *            "idxBinBack": 3,
     *            "idxBinGroup": 0,
     *            "target": "ReagentBin"
     *        },
     *        {
     *            "idxBinBack": 3,
     *            "idxBinGroup": 0,
     *            "target": "ReagentBin"
     *        }
     *    ],
     *    "to": {
     *        "target": "Load"
     *    }
     *}
     */
    static TaskNode Minor_for_PipetteNeedle_DoubleSuction(const QJsonObject& json);

    /**
     * 稀释操作
     */
    static TaskNode Minor_for_Dilute(const QJsonObject& ojson);

    /**
     * 混匀操作
     */
    static TaskNode Minor_for_Mixing(const QJsonObject& ojson);

    /**
     * 单次扫描
     */
    static TaskNode Minor_for_Photoelectric_detect_once(const QJsonObject& json);
};
}  // namespace COM
#endif  // CACTIONMINOR_H_1713408992

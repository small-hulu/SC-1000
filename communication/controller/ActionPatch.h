#ifndef CPATCHMAJOR_H_1713775423
#define CPATCHMAJOR_H_1713775423

#include "../entity/TaskNode.h"

namespace COM {
class ActionPatch {
public:
    static TaskNode Gen_TaskType(const QJsonObject& json);

public:
    /// 针对单个部件的单步操作
    static TaskNode patch_for_Gripper(const QJsonObject& json);
    static TaskNode patch_for_MagneticSeparation(const QJsonObject& json);
    static TaskNode patch_for_PipettingNeedle(const QJsonObject& json);
    static TaskNode patch_for_ReagentBin(const QJsonObject& json);
    static TaskNode patch_for_Mixing(const QJsonObject& json);
    static TaskNode patch_for_Photoelectric(const QJsonObject& json);
    static TaskNode patch_for_Incubate(const QJsonObject& json);
};
}  // namespace COM

#endif  // CPATCHMAJOR_H_1713775423

#ifndef CACTIONMAJOR_H_1713408229
#define CACTIONMAJOR_H_1713408229

#include <QVector>

#include "../entity/TaskNode.h"

namespace COM {
class ActionMajor {
public:
    enum Mode : int {
        Mode_ReadMCUMessage = 0,     /// 获取下位机的信息
        Mode_HomingAllDevice,        /// 所有部件homing
        Mode_HomingAllPump,          /// 所有泵homing
        Mode_TubleDisCard,           /// 所有试管清空
        AbleMagneticSeparationWash,  /// 磁分离的清洗液灌注
        Mode_AbleASubstrate,         /// 底物A灌注
        Mode_AbleBSubstrate,         /// 底物B灌注
        Mode_DrainWash,              /// 排空清洗液
        Mode_Sleep,                  /// 休眠
        Mode_TODO,                   /// 用于紧急添加临时功能

        Mode_Count  /// 此处有两个作用，1)枚举计数 2)非法标志
    };

public:
    static TaskNode Gen_TaskType(const QJsonObject& json);

public:
    /**
     * 读取下位机的信息
     * [无配置整合步骤，json目前无用]
     */
    static TaskNode Major_for_ReadMCUMessage(const QJsonObject& json = {});

    /**
     * 所有部件全部homing
     * [无配置整合步骤，json目前无用]
     */
    static TaskNode Major_for_HomingAllDevice(const QJsonObject& json = {});

    /**
     * 泵归位
     */
    static TaskNode Major_for_HomingAllPump(const QJsonObject& json = {});

    /**
     * 用夹爪丢弃所有的管子
     * [无配置整合步骤，json目前无用]
     */
    static TaskNode Major_for_TubleDisCard(const QJsonObject& json = {});

    /**
     * 磁分离清洗液灌注
     */
    static TaskNode Major_for_AbleMagneticSeparationWash(const QJsonObject& json = {});

    /**
     * 灌注底物A
     */
    static TaskNode Major_for_AbleASubstrate(const QJsonObject& json = {});

    /**
     * 灌注底物B
     */
    static TaskNode Major_for_AbleBSubstrate(const QJsonObject& json = {});

    /**
     * 排空清洗液
     */
    static TaskNode Major_for_DrainWash(const QJsonObject& json = {});

    static TaskNode Major_for_IntensiveWash(const QJsonObject& json = {});

    /**
     * 临时功能，仅仅睡眠
     */
    static TaskNode Major_for_Sleep(const QJsonObject& json = {});

    /**
     * 用于紧急添加临时功能的函数
     */
    static TaskNode Major_for_TODO(const QJsonObject& json = {});
};
}  // namespace COM

#endif  // CACTIONMAJOR_H_1713408229

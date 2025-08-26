#ifndef ACTIONCONTROL_H_1713775423
#define ACTIONCONTROL_H_1713775423

#include "./lte/lte_entity/Task_Node.h"
#include "./lte/controller/HashCalculator.h"
#include "./lte/lte_lbs.h"
///ALL指令控制表
namespace LTE {

class ActionControl {
public:
    static TaskNode Gen_TaskType(const QJsonObject& json);

public:

    static TaskNode Action_for_Table(const QJsonObject& json);
    static TaskNode Action_for_Tcp(const QJsonObject& json);
    static TaskNode Action_for_Hospital_Name(const QJsonObject& json);
    static TaskNode Action_for_Hospital_Departments(const QJsonObject& json);
    static TaskNode Action_for_Upper_Version(const QJsonObject& json);
    static TaskNode Action_for_Lower_Version(const QJsonObject& json);
    static TaskNode Action_for_Map_Key(const QJsonObject& json);

};
}  // namespace LTE

#endif  // ACTIONCONTROL_H_1713775423

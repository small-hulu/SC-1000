#include "ActionControL.h"

#include "../config/CKeyJson.hpp"
#include "controller/CONTROLLER_Config.h"
#include "Parse.h"
#include "log/LOG.h"

namespace LTE {

TaskNode LTE::ActionControl::Gen_TaskType(const QJsonObject &json)
{
    static QHash<QString, TaskStepFactory> table;
    {
        static std::once_flag flag;
        std::call_once(flag, [] {
            table[KJ::ALL_1] = &Action_for_Tcp;
            table[KJ::ALL_2] = &Action_for_Hospital_Name;
            table[KJ::ALL_3] = &Action_for_Hospital_Departments;
            table[KJ::ALL_4] = &Action_for_Upper_Version;
            table[KJ::ALL_5] = &Action_for_Lower_Version;
            table[KJ::ALL_6] = &Action_for_Map_Key;
        });
    }

    const QString mode = json[KJ::actionType].toString();
    if (table.count(mode) == 0) {
        return Parse::parsing_failing(json, __PRETTY_FUNCTION__);
    }
    return table[mode](json);
}

TaskNode ActionControl::Action_for_Tcp(const QJsonObject &json)
{
    auto task = [json]() -> void {
            ///这里应该找到对应对象的 get/set
            if(json[KJ::operate] == KJ::Read)
            {
                auto lte_controler = LTE_ML302::get_instance();
                QString Ip = lte_controler->getIp();
                LTE_LBS::instance().Send_Reply(Ip,json[KJ::hash].toString());
            }
            else if(json[KJ::operate] == KJ::Update)
            {
                LTE_LBS::instance().Send_Reply(KJ::ControlSuccess,json[KJ::hash].toString());
            }

    };
    return {task, json};
}

TaskNode LTE::ActionControl::Action_for_Hospital_Name(const QJsonObject &json)
{
    auto task = [json]() -> void {
            ///这里应该找到对应对象的 get/set
            if(json[KJ::operate] == KJ::Read)
            {
                using namespace CONTROLLER;
                CONTROLLER::Config::Hospital hospital;
                json[KJ::value] = hospital.hospitalName;
                LTE_LBS::instance().Send_Reply(hospital.hospitalName,json[KJ::hash].toString());
            }
            else if(json[KJ::operate] == KJ::Update)
            {
                using namespace CONTROLLER;
                CONTROLLER::Config::Hospital hospital;
                hospital.hospitalName = json[KJ::value].toString();
                hospital.Save();
                LTE_LBS::instance().Send_Reply(KJ::ControlSuccess,json[KJ::hash].toString());
            }

    };
    return {task, json};
}

TaskNode LTE::ActionControl::Action_for_Hospital_Departments(const QJsonObject &json)
{
    auto task = [json]() -> void {
            ///这里应该找到对应对象的 get/set
            if(json[KJ::operate] == KJ::Read)
            {
                using namespace CONTROLLER;
                CONTROLLER::Config::Hospital hospital;
                json[KJ::value] = hospital.departmentName;
                LTE_LBS::instance().Send_Reply(hospital.departmentName,json[KJ::hash].toString());
            }
            else if(json[KJ::operate] == KJ::Update)
            {
                using namespace CONTROLLER;
                CONTROLLER::Config::Hospital hospital;
                hospital.departmentName = json[KJ::value].toString();
                hospital.Save();
                LTE_LBS::instance().Send_Reply(KJ::ControlSuccess,json[KJ::hash].toString());
            }

    };
    return {task, json};
}

TaskNode LTE::ActionControl::Action_for_Upper_Version(const QJsonObject &json)
{
    auto task = [json]() -> void {

    };
    return {task, json};
}

TaskNode LTE::ActionControl::Action_for_Lower_Version(const QJsonObject &json)
{
    auto task = [json]() -> void {

    };
    return {task, json};
}

TaskNode LTE::ActionControl::Action_for_Map_Key(const QJsonObject &json)
{
    auto task = [json]() -> void {
            ///这里应该找到对应对象的 get/set
            if(json[KJ::operate] == KJ::Read)
            {
                auto lte_controler = LTE_ML302::get_instance();
                QString key = lte_controler->getKey();
                LTE_LBS::instance().Send_Reply(key,json[KJ::hash].toString());
            }
            else if(json[KJ::operate] == KJ::Update)
            {
                auto lte_controler = LTE_ML302::get_instance();
                lte_controler->setKey(json[KJ::value].toString());
                LTE_LBS::instance().Send_Reply(KJ::ControlSuccess,json[KJ::hash].toString());
            }

    };
    return {task, json};
}

TaskNode LTE::ActionControl::Action_for_Table(const QJsonObject &json)
{
    auto task = [json]() -> void {

    };
    return {task, json};
}

}  // namespace COM

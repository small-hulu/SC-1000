/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       CAnalyse.cpp
 * @date       2024-03-25
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief
 * json.value(key) 没有这个key默认undefined
 * toInt() 为0
 * toString() 为""
 * 不会抛出异常
 *
 *****************************************************************************/
#include "Parse.h"

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QThread>
#include <future>

namespace LTE {

///包装Tasknode
TaskNode Parse::json_to_task(const QJsonObject& json, bool& ok) {
    const QString actionType = json[LTE::KJ::actionType].toString();

    ok = true;
    if (false) {
    } else if (actionType.contains(KJ::ALL)) {
        return ActionControl::Gen_TaskType(json);
    } else if (actionType.contains(KJ::SC1000)) {
        return ActionControl::Gen_TaskType(json);
    }

    ok = false;
    return parsing_failing(json, "《文本配置解析失败》");
}

TaskNode Parse::parsing_failing(const QJsonObject& json, const QString& msg) {
    auto task = [json, msg]() -> void {

    };
    return {task, json};
}
}  // namespace COM

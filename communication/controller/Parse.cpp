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

#include "../config/CKeyJson.hpp"
#include "ActionMajor.h"
#include "ActionMinor.h"
#include "ActionPatch.h"

namespace COM {

TaskNode Parse::json_to_task(const QJsonObject& json, bool& ok) {
    const QString action = json[KJ::actionType].toString();

    ok = true;
    if (false) {
    } else if (action == KJ::ActionMajor) {
        return ActionMajor::Gen_TaskType(json);
    } else if (action == KJ::ActionMinor) {
        return ActionMinor::Gen_TaskType(json);
    } else if (action == KJ::ActionPatch) {
        return ActionPatch::Gen_TaskType(json);
    }

    ok = false;
    return parsing_failing(json, "《文本配置解析失败》");
}

TaskNode Parse::parsing_failing(const QJsonObject& json, const QString& msg) {
    auto task = [json, msg](Tube::entity_tube) -> TaskNode::return_type {
        qWarning() << "%文本配置解析失败%" << msg << RetCode::TextParsingError << json;
        return TaskNode::return_type::TextParsingError;
    };
    return {task, json};
}

/**
 * 根据json数组中的，每个元素的key，找出对应的相应规则
 */
Parse::task_sequence Parse::parsing_process_node(const QJsonObject& json) {
    if (json.isEmpty()) {
        qWarning() << RetCode::TextParsingError << "%文本配置解析为空%";
        return {};
    }

    task_sequence taskList;
    QJsonArray    jStepsArr = json[KJ::processSteps].toArray();
    for (int i = 0; i < jStepsArr.count(); i += 1) {
        QJsonObject json = jStepsArr[i].toObject();
        bool        ok   = true;
        taskList << json_to_task(json, ok);
        if (ok == false) {
            qWarning() << "解析错误";
        }
    }
    return taskList;
}

/**
 * 转发到双参函数中
 */
bool Parse::simple_single_process(Parse::task_sequence tasks) {
    bool keepRuning = true;
    return simple_single_process(std::move(tasks), keepRuning);
}

/**
 * 目前操作是一步失败后，就直接失败
 */
bool Parse::simple_single_process(Parse::task_sequence tasks, bool& keepRuning) {
    auto pipe = Tube::Create_new_SmartPipe();
    for (int i = 0; i < tasks.size() && keepRuning; i += 1) {
        auto& task = tasks[i];
        qDebug().noquote() << "$$$开始执行任务：Task-Message = " << task.to_string();

        TaskNode::return_type ret{};
        {
            ret = task(pipe);
            pipe->Append_strMsg(ret.to_string() + task.to_string());
        }
        keepRuning &= (ret == RetCode::Success);

        qDebug() << "result = " << ret;
    }

    QString msg;
    QDebug  debug(&msg);
    debug.nospace() << "*****************************************************\n";
    debug.nospace() << "*** 流程运行结束 *** " << keepRuning << "\n";
    debug.nospace() << "*****************************************************\n";
    qDebug().noquote() << msg;

    return keepRuning;
}

bool Parse::simple_single_process(Parse::task_sequence tasks, std::function<bool(int)> check) {
    auto pipe = Tube::Create_new_SmartPipe();

    bool keepRuning = true;
    for (int i = 0; i < tasks.size() && keepRuning; i += 1) {
        auto& task = tasks[i];
        qDebug().noquote() << "$$$开始执行任务：Task-Message = " << task.to_string();

        TaskNode::return_type ret{};
        {
            ret = task(pipe);
            pipe->Append_strMsg(ret.to_string() + task.to_string());
        }
        keepRuning &= check((int)ret.result);

        qDebug() << "result = " << ret;
    }

    QString msg;
    QDebug  debug(&msg);
    debug.nospace() << "*****************************************************\n";
    debug.nospace() << "*** 流程运行结束 *** " << keepRuning << "\n";
    debug.nospace() << "*****************************************************\n";
    qDebug().noquote() << msg;

    return keepRuning;
}
}  // namespace COM

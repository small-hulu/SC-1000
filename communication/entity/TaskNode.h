#ifndef CJSONANALYSE_H_1711691694
#define CJSONANALYSE_H_1711691694

#include <QJsonDocument>
#include <QJsonObject>
#include <mutex>
#include <unordered_set>
#include <vector>

#include "../config/CRetCode.h"
#include "Tube.h"

namespace COM {
class TaskNode {
public:
    using task_type   = std::function<RetCode(Tube::entity_tube)>;
    using return_type = task_type::result_type;

    enum State {
        Invalid,     // 无效
        ParseError,  // 解析错误
        Again,       // 重试
        Pass         // 通过
    };

private:
    /// 实际的任务
    task_type m_task;

private:
    QJsonObject m_ojson;
    QString     m_message;

public:
    TaskNode() = default;
    TaskNode(task_type task) : m_task(std::move(task)) {}
    TaskNode(task_type task, QJsonObject json)
        : m_task(std::move(task)), m_ojson(std::move(json)) {}

    TaskNode(const TaskNode&)            = default;
    TaskNode(TaskNode&&)                 = default;
    TaskNode& operator=(const TaskNode&) = default;
    TaskNode& operator=(TaskNode&&)      = default;

public:  // to_string
    QString to_string() const {
        return QString::fromUtf8(QJsonDocument(m_ojson).toJson(QJsonDocument::Indented));
    }

public:  // 目的就是为了包装这个任务
    RetCode operator()(Tube::entity_tube pipe) const {
        assert(m_task);
        return m_task(pipe);
    }
};

using TaskStepFactory = TaskNode (*)(const QJsonObject&);

}  // namespace COM
#endif  // CJSONANALYSE_H_1711691694

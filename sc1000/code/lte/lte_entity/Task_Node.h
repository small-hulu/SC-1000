#ifndef LTE_TASK_NODE_H_1711691694
#define LTE_TASK_NODE_H_1711691694

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <functional>
#include <cassert>

namespace LTE {

class TaskNode {
public:
    using task_type = std::function<void(void)>;

private:
    task_type   m_task;
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

public:

    QString to_string() const {
        return QString::fromUtf8(QJsonDocument(m_ojson).toJson(QJsonDocument::Indented));
    }

    //包装 TaskNode
    void operator()() const {
        assert(m_task);
        m_task();
    }

    explicit operator bool() const noexcept {
        return static_cast<bool>(m_task);
    }

    const QJsonObject& json() const { return m_ojson; }
};

using TaskStepFactory = TaskNode (*)(const QJsonObject&);

}  // namespace LTE

#endif  // LTE_TASK_NODE_H_1711691694

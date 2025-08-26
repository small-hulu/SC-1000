#ifndef CANALYSIS_H_1711337457
#define CANALYSIS_H_1711337457

#include <QJsonObject>
#include <QList>

#include "../entity/TaskNode.h"

namespace COM {
class Parse final {
public:
    /// 任务序列
    /// 从json中生成一个线程的序列
    using task_sequence = QList<TaskNode>;

public:
    static TaskNode json_to_task(const QJsonObject& json, bool& ok);

public:
    /**
     * 解析错误，也包装成一个函数
     */
    static TaskNode parsing_failing(const QJsonObject& json, const QString& msg);

    /**
     * 根据指定的规则，将json解析成线性执行的任务
     * Mediator 中介者
     */
    static task_sequence parsing_process_node(const QJsonObject& json);

    /**
     * 顺序运行步骤
     * 设计两个重载
     * 1. 是内部正常运行完
     * 2. 外部可以用一个flag进行中断
     */
    static bool simple_single_process(task_sequence tasks);
    static bool simple_single_process(task_sequence tasks, bool& keepRuning);
    static bool simple_single_process(task_sequence tasks, std::function<bool(int)> check);
};
}  // namespace COM

#endif  // CANALYSIS_H_1711337457

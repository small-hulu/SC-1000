#ifndef CANALYSIS_H_1711337457
#define CANALYSIS_H_1711337457

#include <QJsonObject>
#include <QList>
#include "lte/lte_entity/Task_Node.h"
#include "./lte/config/CKeyJson.hpp"
#include "./lte/controller/ActionControl.h"
namespace LTE {
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
;
};
}  // namespace LTE

#endif  // CANALYSIS_H_1711337457

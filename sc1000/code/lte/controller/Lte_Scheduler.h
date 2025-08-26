#ifndef LTE_SCHEDULER_H
#define LTE_SCHEDULER_H
#include "lte/interface/ISingleton.hpp"
#include <queue>
#include "lte/lte_entity/Task_Node.h"
#include "lte/lte.h"
namespace LTE {

class Lte_Scheduler : public ISingleton<Lte_Scheduler>
        , public QThread
{
public:
    Lte_Scheduler();
    ~Lte_Scheduler();
public:
    using task_type = LTE::TaskNode;
public:
    void Start() {
        th_isRunning  = true;
        start();
    }
private:
    std::atomic<bool>       th_isRunning = false;
    mutable std::mutex      th_taskMutex;
    std::condition_variable th_taskCV;

    std::queue<task_type> m_taskQueue;

public:
    void Append_Task(const task_type& task) {
        {
            std::unique_lock<std::mutex> lock(th_taskMutex);
            m_taskQueue.emplace(task);
        }
        th_taskCV.notify_one();
    }
private:
    bool m_initSuccess = false;

protected:
    virtual void run() override;
};

}
#endif // LTE_SCHEDULER_H

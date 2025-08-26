#include "Lte_Scheduler.h"
#include "./lte/controller/HashCalculator.h"
#include "./lte/controller/Parse.h"
#include "./lte/lte_lbs.h"
#include "./lte/config/CKeyJson.hpp"
namespace LTE {

Lte_Scheduler::Lte_Scheduler()
{
    setObjectName("LTE调度器");
    th_isRunning = true;
    m_initSuccess = true;
}

Lte_Scheduler::~Lte_Scheduler()
{
    th_isRunning = false;
    th_taskCV.notify_all();
    if (this->isRunning()) {
        qInfo() << QString("~QThread(%1) thread-quit").arg(objectName());
        this->quit();
        this->wait();
    }
}

void Lte_Scheduler::run()
{
    while (m_initSuccess && th_isRunning) {
        task_type task{};
        {
            qDebug()<<"Lte_Scheduler thread"<<endl;
            std::unique_lock<std::mutex> lock(th_taskMutex);
            while (m_taskQueue.size() == 0 && th_isRunning) {
                th_taskCV.wait(lock);
            }
            if (th_isRunning == false) {
                break;
            }
            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
        }
        if (task) {
            task();
        }
    }
    qInfo() << "END " << __PRETTY_FUNCTION__;
}


}

#include "lte_lbs.h"

LTE_LBS::LTE_LBS(QObject *parent) : QThread(parent) {
    th_isRunning = true;
}

LTE_LBS::~LTE_LBS() {
    th_isRunning = false;
    th_taskCV.notify_all();
    if (this->isRunning()) {
        qInfo() << QString("~QThread(%1) thread-quit").arg(objectName());
        this->quit();
        this->wait();
    }

    LTE_ML302::get_instance()->close_serial_port();
}

void LTE_LBS::run() {
    while (m_initSuccess && th_isRunning) {
        task_type task{};
        {
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

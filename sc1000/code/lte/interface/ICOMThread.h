#ifndef ICOMTHREAD_HPP_1724744170
#define ICOMTHREAD_HPP_1724744170

#include <QDebug>
#include <QThread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <thread>

class ICOMThread : public QThread {
    Q_OBJECT
public:
    std::atomic<bool>       th_isRunning = false;
    mutable std::mutex      th_taskMutex;
    std::condition_variable th_taskCV;

public:
    using QThread::QThread;

    virtual ~ICOMThread();

public:
    /**
     * 启动 run loop
     * 请保证已经初始化过fun了
     */
    virtual void Start_thread();

    /**
     * 停止 run loop
     */
    virtual void Stop_thread();

    /**
     * 清理线程中的资源
     */
    virtual void Clear_threadResource();
};

#endif  // ICOMTHREAD_HPP_1724744170

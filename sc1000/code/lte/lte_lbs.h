#ifndef LTE_LBS_H_1740101620
#define LTE_LBS_H_1740101620

#include <QThread>
#include <functional>
#include <queue>

#include "controller/CONTROLLER_Config.h"
#include "controller/IniSC1000.h"
#include "entity/ENTITY_EExpSample.h"
#include "lte/lte.h"
#include "lte_sc.h"

class LTE_LBS : public QThread {
    Q_OBJECT
public:
    using task_type = std::function<void()>;

public:
    using Self = LTE_LBS;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

private:
    QString m_vid;
    QString m_pid;

private:
    std::atomic<bool>       th_isRunning = false;
    mutable std::mutex      th_taskMutex;
    std::condition_variable th_taskCV;

    std::queue<task_type> m_taskQueue;

private:
    bool m_initSuccess = false;

private:
    LTE_LBS(QObject* parent = nullptr);
    ~LTE_LBS();

public:
    void Start() {
        th_isRunning  = true;

        m_initSuccess = LTE_SC::Init(m_vid, m_pid);
        if (m_initSuccess) {
            qInfo() << "4G模块连接成功！";
            start();
        } else {
            qWarning() << "4G模块连接失败！";
        }
    }

    void Set_vid_pid(const QString& vid, const QString& pid) {
        m_vid = vid;
        m_pid = pid;
    }

public:
    ///发送控制指令
    ///reply
    void Send_Reply(QString data,QString md5) {
        if (m_initSuccess) {
            Append_Task(std::bind(&LTE_SC::Send_SC_Reply, data, md5));
            //LTE_SC::Send_SC_Reply(data,md5);
        }
    }
    void Send_Data(std::shared_ptr<ENTITY::EExpSample> expItem) {

        if (m_initSuccess) {
            Append_Task(std::bind(&LTE_SC::Send_SC_Data, expItem));
            //LTE_SC::Send_SC_Data(expItem);
        }
    }

    void Send_ErrorCode(LTE_SC::ErrorCode code) {
        if (m_initSuccess) {
            Append_Task(std::bind(&LTE_SC::Send_SC_ErrorCode, code));
            //LTE_SC::Send_SC_ErrorCode(code);
        }
    }

    void Append_Task(const task_type& task) {
        {
            std::unique_lock<std::mutex> lock(th_taskMutex);
            m_taskQueue.emplace(task);
        }
        th_taskCV.notify_one();
    }

protected:
    virtual void run() override;
};

#endif  // LTE_LBS_H_1740101620

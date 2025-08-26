/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DeviceOpAuthorizeCU.h
 * @date       2024-06-18
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 设备所有权控制单元
 *
 *****************************************************************************/
#ifndef DEVICEOPAUTHORIZECU_H_1718679907
#define DEVICEOPAUTHORIZECU_H_1718679907

#include <QDateTime>
#include <QJsonObject>
#include <QList>
#include <QThread>
#include <atomic>
#include <limits>
#include <mutex>
#include <queue>

#include "../device/DeviceManager.h"
#include "../interface/ICOMThread.h"
#include "../interface/ISingleton.hpp"

namespace COM {

class MultLock final {
public:
    /// 优先级
    using priority_type = int;

    enum priority_level : priority_type {
        Level_0 = std::numeric_limits<priority_type>::max() / 2,
        Level_1 = std::numeric_limits<priority_type>::max() / 3,
        Level_2 = std::numeric_limits<priority_type>::max() / 4,
        Level_3 = std::numeric_limits<priority_type>::max() / 5,
        Level_4 = std::numeric_limits<priority_type>::max() / 6,  //数值最小，优先级最高
    };

    /**
     * 解析优先级
     */
    static priority_type Resolve_Priority(const QJsonObject& json, priority_type level);

public:
    enum class State : int {
        Invalid,   // 非法
        Locking,   // 上锁中
        Success,   // 成功获取
        Again,     // 再次尝试
        Pause,     // 停止
        Unlocked,  // 解锁完成
    };

public:
    using entity_multLock = std::shared_ptr<MultLock>;
    using ownership_list  = QList<DeviceManager::ownership_type*>;

public:
    static entity_multLock Create_entity(const ownership_list& list, priority_type level = 0,
                                         const char* const func = __builtin_FUNCTION());

    static entity_multLock Create_entity(const QJsonObject& json, priority_type level = 0,
                                         const char* const func = __builtin_FUNCTION());

public:
    std::atomic<State> m_state{State::Invalid};
    ownership_list     mtxList;
    priority_type      m_level = 0;
    QString            m_fromFunc;
    QThread*           m_fromThread;

public:
    MultLock() = default;
    ~MultLock();

    bool operator<(const MultLock& rhs) const {
        return this->m_level < rhs.m_level;
    }

public:
    QString to_string() const;

public:
    bool try_once();
};

class DeviceOpAuthorizeCU final
    : public ICOMThread
    , public ISingleton<DeviceOpAuthorizeCU> {
public:
    friend MultLock;

    /**
     * 控制器的屏蔽层栅栏
     * 目前基于时间和节点 level 决定
     */
    struct CUFence {
        std::mutex              mutex;
        MultLock::priority_type level = -1;
        QDateTime               time  = QDateTime{};

        void Reset() {
            level = -1;
            time  = QDateTime{};
        }
        bool IsVaild() const {
            return level >= 0 && time.isValid() == true;
        }

        QString to_string() const {
            QString s;
            QDebug  os(&s);
            os << "level = " << level << "\n";
            os << "time = " << time << "\n";
            os << "IsVaild = " << IsVaild() << "\n";
            return s;
        }
    };

private:
    QList<MultLock::entity_multLock> m_taskQueue;
    CUFence                          m_fench;

public:
    DeviceOpAuthorizeCU();

public:
    QString to_string() const;

public:
    void Append_Task(MultLock::entity_multLock entity) {
        {
            std::unique_lock<std::mutex> lock(th_taskMutex);
            m_taskQueue << entity;
            qSort(m_taskQueue.begin(), m_taskQueue.end(),
                  [](auto&& a, auto&& b) { return !(*a < *b); });  //不同task的同一时刻的node优先级排序，优先级高的node会先被处理掉
        }
        th_taskCV.notify_one();
    }

public:
    //更新fench里的优先级和有效期
    void PutUp_fench(const CUFence& fench) {
        std::unique_lock<std::mutex> lock(m_fench.mutex);
        qInfo().noquote().nospace()
            << __func__ << "m_fench = " << m_fench.to_string() << " fench = " << fench.to_string();

        m_fench.level = qMax<int>(m_fench.level, fench.level);
        m_fench.time  = qMax(m_fench.time, fench.time);
    }

    void Headle_fench() {
        //std::unique_lock是一个可以灵活管理锁的RAII（Resource Acquisition Is Initialization）对象。
        //它会在函数结束时自动释放锁（如果没有异常），保证锁的正确释放，避免死锁。
        std::unique_lock<std::mutex> lock(m_fench.mutex);
        if (m_fench.IsVaild() == false) {
            return;
        }
        auto curTime = QDateTime::currentDateTime();//fench有效期过了就重置 m_fench
        if (m_fench.time <= curTime) {
            qInfo().noquote().nospace() << __func__ << "m_fench = " << m_fench.to_string();
            m_fench.Reset();
        }
    }

protected:
    virtual void run() override;
};
}  // namespace COM

#endif  // DEVICEOPAUTHORIZECU_H_1718679907

#include "DeviceOpAuthorizeCU.h"

#include <QDebug>
#include <QJsonDocument>
#include <limits>

#include "../config/CKeyJson.hpp"

#define ON_DEBUG_MODBUS 0

#define DEBUG_MODBUS()      \
    if (!ON_DEBUG_MODBUS) { \
    } else                  \
        qDebug()

namespace COM {

MultLock::priority_type MultLock::Resolve_Priority(const QJsonObject      &json,
                                                   MultLock::priority_type level) {
    constexpr auto TopLevel = priority_level::Level_1;

    auto from   = json[KJ::from].toObject();
    auto target = from[KJ::target].toString();
    /// todo
    /// 单独处理从温裕盘转移走的优先级提高
    if (target == KJ::Incubate) {
        return qMax<MultLock::priority_type>(level, TopLevel);
    } else {
        return level;
    }
}
//list为要锁的设备的列表
MultLock::entity_multLock MultLock::Create_entity(const MultLock::ownership_list &list,
                                                    priority_type level, const char *const func)
{
    auto entity = entity_multLock(new entity_multLock::element_type{});

    entity->m_level      = level;                     // 当前json node的level
    entity->mtxList      = list;                      // 要锁的资源列表，比如 Gripper, Cup, CriticalZone...
    entity->m_fromFunc   = func;
    entity->m_fromThread = QThread::currentThread();  // 记录当前线程

    assert(entity->mtxList.size());
    entity->m_state = State::Locking;

    DeviceOpAuthorizeCU::instance().Append_Task(entity);  // 把抢锁任务加入中央授权管理器

    //阻塞当前线程直到抢锁成功
    while (entity->m_state != State::Success) {
        QThread::yieldCurrentThread();  // 自旋等待，让出CPU，直到 m_state = State::Success
    }

    DEBUG_MODBUS() << entity.get() << "lock finished";
    return entity;
}

MultLock::entity_multLock MultLock::Create_entity(const QJsonObject &json, priority_type level,
                                                  const char *const func) {
    return Create_entity(DeviceManager::Analyze_ownerList(json), level, func);
}

MultLock::~MultLock() {
    if (m_state == State::Success) {
        for (auto &&owner : mtxList) {
            if (owner->load() == false) {
                qCritical() << QThread::currentThread()
                            << " atomic-state-error &atomic = " << owner;
            } else {
                owner->store(false); //释放设备资源锁
            }
        }
        m_state = State::Unlocked;
    } else if (m_state == State::Invalid) {
        qWarning() << "无效锁";
        /// pass
    } else {
        qWarning() << "未上锁成功，失效。";
    }

    DEBUG_MODBUS() << this << "unlock finished";
}

QString MultLock::to_string() const {
    QJsonObject json;
    json["State"]      = (int)m_state.load();
    json["level"]      = m_level;
    json["fromfunc"]   = m_fromFunc;
    json["fromThread"] = QString::asprintf("%p", m_fromThread);
    for (int i = 0; i < mtxList.size(); i += 1) {
        json[QString("list[%1]").arg(i)] = QString::asprintf("%p", mtxList[i]);
    }

    return QString::fromUtf8(QJsonDocument(json).toJson(QJsonDocument::Indented));
}

//尝试原子性上锁多个设备的函数，确保要么全部成功，要么全部失败并回滚。
bool MultLock::try_once() {
    int cnt = 0;
    for (int i = 0; i < mtxList.size(); i += 1) {
        bool old = false;
        //compare_exchange_strongwe为对 mtxList 的原子操作函数 给mtxList里的所有资源上锁
        if (mtxList[i]->compare_exchange_strong(old, true)) {
            cnt += 1;
        } else {
            break;
        }
    }

    if (cnt != mtxList.size()) {
        /// 保证前驱的被锁上了
        for (int i = 0; i < cnt; i += 1) {
            *mtxList[i] = false;
        }
        return false;
    }

    m_state = State::Success;
    return true;
}

///////////////////////////////////////////////////////////
DeviceOpAuthorizeCU::DeviceOpAuthorizeCU() : ICOMThread() {
    setObjectName("锁控制器");
    this->Start_thread();
}

QString DeviceOpAuthorizeCU::to_string() const {
    std::unique_lock<std::mutex> lock(th_taskMutex);

    QString s;
    QDebug  os(&s);
    os << "thread-isrunning = " << isRunning() << "\n";
    os << "fench = " << m_fench.to_string() << "\n";
    os << "task-size = " << m_taskQueue.size() << "\n";
    for (auto task : m_taskQueue) {
        os << task->to_string() << "\n";
    }
    return s;
}

void DeviceOpAuthorizeCU::run() {
    while (th_isRunning) {
        QThread::yieldCurrentThread();

        std::unique_lock<std::mutex> lock(th_taskMutex);
        while (m_taskQueue.size() == 0 && th_isRunning) {
            th_taskCV.wait(lock);
        }
        if (false == th_isRunning) {
            break;
        }

        Headle_fench();

        for (int i = 0; i < m_taskQueue.size(); i += 1) {
            auto &&task = m_taskQueue[i];
            //fench作用
            if (m_fench.IsVaild() && task->m_level < m_fench.level) {
                break;
            }
            if (task->try_once()) {
                m_taskQueue.removeAt(i);
                break;
            }
        }
    }  // while

    qWarning() << objectName() << "run end";
}

}  // namespace COM

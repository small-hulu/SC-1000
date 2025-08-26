#include "Tube.h"

#include <QDebug>
#include <QThread>

#include "../controller/DeviceOpAuthorizeCU.h"
#include "../device/DeviceManager.h"

namespace COM {
/**
 * @brief EPipe::EPipe
 * 记录初始信息
 * + 时间
 */
Tube::Tube() {
    Append_timeMsg();

    QString msg;
    QDebug  qos(&msg);
    qos.nospace() << __PRETTY_FUNCTION__ << this << QThread::currentThread() << "\n";
    qos.nospace() << QThread::currentThread();
    Append_strMsg(msg);

    m_state = State_CanBeTransfer;
}

/**
 * @brief EPipe::~EPipe
 * 记录所有日志
 */
Tube::~Tube() {
    Append_timeMsg();
    // qInfo().noquote().nospace() << to_string();
}

QString Tube::to_string() const {
    QString msg;
    QDebug  qos(&msg);
    for (auto&& msg : m_strMsgList) {
        qos.noquote().nospace() << msg << "\n";
    }
    for (auto&& t : m_timeMsgList) {
        qos.noquote().nospace() << t << "\n";
    }
    qos << "ExpValue = " << m_expValue;

    return msg;
}

void Tube::Set_delayTimeTask(int targetTime, std::function<void()> task) {
    m_delayPackage.targetTime = targetTime;
    m_delayPackage.task       = task;

    QTimer::singleShot(0, QThread::currentThread(),
                       [this] { m_delayPackage.timeId = startTimer(m_delayPackage.timeInterval); });
}

void Tube::timerEvent(QTimerEvent* event) {
    constexpr int intervalTime  = 5000; // 固定值，提前 5 秒做授权准备
    m_delayPackage.cntTime     += m_delayPackage.timeInterval;

    auto exec_tack = [this] {
        m_delayPackage.task();              // 执行用户指定的任务
        killTimer(m_delayPackage.timeId);  // 停止定时器
        m_delayPackage.Reset();            // 重置状态
    };


    if (m_delayPackage.cntTime >= m_delayPackage.targetTime) {
        exec_tack();
        return;
    }
    else if (m_delayPackage.isAdjusted == false &&
               m_delayPackage.cntTime + intervalTime >= m_delayPackage.targetTime)
    {
        m_delayPackage.isAdjusted = true;

        DeviceOpAuthorizeCU::CUFence fence;
        fence.time  = QDateTime::currentDateTime().addMSecs(intervalTime); // 设置授权有效时间
        fence.level = MultLock::priority_level::Level_4; // 设置授权优先级为最高优先级
        DeviceOpAuthorizeCU::instance().PutUp_fench(fence);

    }
}

}  // namespace COM

/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       IDeviceThread.cpp
 * @date       2024-03-29
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief
 * 注意，千万不要使用`std::this_thread::yield()`
 * 理论上是会让出调度权的，但实际在x86平台下直接卡死
 *
 *****************************************************************************/

#include "IDeviceThread.h"

#include <QDebug>

#include "ModbusImpl.h"

#define DEBUG_THREAD()      \
    if (!ON_DEBUG_THREAD) { \
    } else                  \
        qDebug()

#define ON_DEBUG_THREAD (1)

namespace COM {

IDeviceThread::IDeviceThread(QObject* parent) : ICOMThread(parent) {
    setObjectName(__func__);
}

/**
 * once fun
 * 设置任务函数
 */
void IDeviceThread::set_loopFun(QVector<AtomicTask> setFun, QVector<AtomicTask> checkFun) {
    execSetFuncList   = std::move(setFun);
    execCheckFuncList = std::move(checkFun);
    assert(execSetFuncList.size());
    assert(execSetFuncList.size() == execCheckFuncList.size());
}
/**
 * 操作单次，并行
 */
void IDeviceThread::Exec_parallelTask() {
    m_isLooping = false;
    QThread::msleep(PeriodMSInterval_Task * 2.5);
    m_isLooping = true;
    th_taskCV.notify_one();
}

/**
 * 操作单次，串行
 */
RetCode IDeviceThread::Exec_sequencedTask() {
    static std::unique_lock<std::mutex> lock;
    return operator_task(lock); //lock is fasle
}

/**
 * 判断lock是否有对应的 mutex 来区分串行还是并行
 * set   模式
 * check 结束
 * check error
 */
RetCode IDeviceThread::operator_task(std::unique_lock<std::mutex>& lock) {
    QString policy  = lock.owns_lock() ? QString("LOOP") : QString("ONCE");
    QString message = QString(">>>[%2]mode[%3]{[%4]}")  //
                                                        //.arg(policy)                      //
                          .arg(objectName())            //
                          .arg(m_modeIndex);

    assert(m_modeIndex < execSetFuncList.size());
    auto&&  setFunc   = execSetFuncList[m_modeIndex];
    auto&&  checkFunc = execCheckFuncList[m_modeIndex];
    RetCode success   = RetCode::Running;

    if (ModbusImpl::instance().isOpen() == false) {
        DEBUG_THREAD() << message.arg("modbus 连接异常");
        return RetCode::ModbusError;
    }

    if (setFunc) {
        DEBUG_THREAD() << "[1]" << message.arg("Op-set begin");
        //! 串行
        if (lock.owns_lock() == false) {
            success = setFunc();
        }
        //! 并行
        else {
            if (m_isLooping) {
                success = setFunc();
            }
        }
        DEBUG_THREAD() << "[2]" << message.arg("Op-set end") << success;
    }

    /// set成功，才会执行check操作
    if ((success == RetCode::Success) && checkFunc) {
        DEBUG_THREAD() << "[3]" << message.arg("Op-check start");

        //! 串行
        if (lock.owns_lock() == false) {
            while (RetCode::Running == (success = checkFunc())) {
                QThread::yieldCurrentThread();
            }
        }
        //! 并行
        else {
            while (m_isLooping && (RetCode::Running == (success = checkFunc()))) {
                lock.unlock();
                QThread::yieldCurrentThread();
                lock.lock();
            }
        }

        DEBUG_THREAD() << "[4]" << message.arg("Op-check end") << success;
    }

    return success;
}

void IDeviceThread::run() {
    const int Fun_Count = execSetFuncList.size();
    assert(Fun_Count);

    while (th_isRunning) {
        std::unique_lock<std::mutex> lock(th_taskMutex);
        while (m_isLooping == false && th_isRunning) {
            th_taskCV.wait(lock);
        }
        if (th_isRunning == false) {
            break;
        }

        /// m_isLooping == true
        operator_task(lock);

        if (m_isLooping == false) {
            DEBUG_THREAD() << "强制终止该次操作";
        }
        m_isLooping = false;
    }
}
}  // namespace COM

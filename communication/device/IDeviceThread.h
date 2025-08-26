/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       IEquipThread.h
 * @date       2024-03-13
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      操作的线程任务类
 * 这里纯线程操作，与具体的业务细节分离
 *
 *****************************************************************************/
#ifndef WORKERTHREAD_H_1705370025
#define WORKERTHREAD_H_1705370025

#include <QThread>
#include <QVector>
#include <functional>

#include "../config/CParams.hpp"
#include "../config/CRetCode.h"
#include "../interface/ICOMThread.h"

namespace COM {
class IDeviceThread : public ICOMThread {
    Q_OBJECT
protected:
    /// 基于协议的基本原子任务
    using AtomicTask = std::function<RetCode(void)>;

protected:  // 运行间隔周期
    static constexpr int PeriodMSInterval_Task = 20;

protected:  // 执行任务
    std::atomic<int>    m_modeIndex{0};
    QVector<AtomicTask> execSetFuncList;
    QVector<AtomicTask> execCheckFuncList;

protected:  // 事件循环
    std::atomic<bool> m_isLooping{false};

public:
    IDeviceThread(QObject* parent = nullptr);

protected:  /// for build
    /**
     * @brief set_loopFun
     * @param setFun        设定运作模式（）执行操作函数
     * @param checkFun      检测操作是否结束，只是能够结束，不代表运行成功
     * Builder 型函数
     * set和check的长度要保持一致
     */
    void set_loopFun(QVector<AtomicTask> setFun, QVector<AtomicTask> checkFun);

public:
    int get_currentMode() {
        return m_modeIndex.load();
    }

public:
    /**
     * 执行并行任务
     * 先保证已经set了fun&mode
     */
    void Exec_parallelTask();

    /**
     * 执行串行任务
     * 先保证已经set了fun&mode
     */
    RetCode Exec_sequencedTask();

private:
    /**
     * 统一串行和并行策略
     */
    RetCode operator_task(std::unique_lock<std::mutex>& lock);

protected:
    /**
     * 在本类 final ，子类只能设置函数和触发
     * 线程逻辑统一在这里
     */
    void run() override final;
};
}  // namespace COM

#endif  // WORKERTHREAD_H_1705370025

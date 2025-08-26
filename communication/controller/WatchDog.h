#ifndef WATCHDOG_H_1711512619
#define WATCHDOG_H_1711512619

#include <QThread>
#include <QVector>

#include "../interface/ICOMThread.h"
#include "../interface/ISingleton.hpp"
#include "../utility/IniConfig.h"

namespace COM {
class WatchDog
    : public ICOMThread
    , public ISingleton<WatchDog> {
    Q_OBJECT
    friend class ISingleton<WatchDog>;

private:
    constexpr static int Loop_Interval = 1500;

public:
    using SelfTask = std::function<void(void)>;

private:
    QList<SelfTask> m_watchedTaskList;

protected:
    WatchDog(QObject* parent = nullptr);

public:
    void Reset();

public:
    void Append_watchTask(const SelfTask& task);

private:
    /// 检测错误
    void check_mcuError();

    /// 检测耗材
    void check_consumable();

    /// 检测上料通道是否有料
    void check_loadMaterial();

    /// 排液使能
    void exec_drainage();

    /// 试剂仓旋转
    void exec_reagentBinSpinLoop();

protected:
    virtual void run() override;
};

/**
 * 范围停止监控
 */
struct WatchDog_Guard {
    WatchDog_Guard() {
        WatchDog::instance().Stop_thread();
    }
    ~WatchDog_Guard() {
        WatchDog::instance().Start_thread();
    }
};

/**
 * 范围启动监控
 */
struct WatchDog_AntiGuard {
    WatchDog_AntiGuard() {
        WatchDog::instance().Start_thread();
    }
    ~WatchDog_AntiGuard() {
        WatchDog::instance().Stop_thread();
    }
};
}  // namespace COM

#endif  // WATCHDOG_H_1711512619

#ifndef WIDGET_MTOP_TOPEXPSTATE_H_1725864233
#define WIDGET_MTOP_TOPEXPSTATE_H_1725864233

#include <QObject>
#include <QString>

#include "communication/controller/DeviceOpAuthorizeCU.h"

namespace WIDGET::MAIN {
class ExpStateController : public QObject {
    Q_OBJECT
public:
    inline static const int Time_Interval_pause_byDevice = 5 * 60 * 1000;

    enum class ExpState {
        idle,            // 空闲
        running,         // 运行中
        pause_byUser,    // 暂停-用户主动
        pause_byDevice,  // 暂停-设备
        stop,            // 停止
    };

    Q_ENUM(ExpState);

private:
    struct Info {
        bool    ok = false;
        QString msg;
    };

public:
    using Self = ExpStateController;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

private:
    ExpState                       state{};
    COM::MultLock::entity_multLock lock_pause_byUser;
    COM::MultLock::entity_multLock lock_pause_byDevice;
    QTimer                         timer_pause_byDevice;

private:
    ExpStateController();

public:
    ~ExpStateController();

public:
    void Reset();

public:
    ExpState Get_State() {
        return state;
    }
    void Set_State(ExpState newState);

public:
    void Exp_start();
    void Exp_pause();
    void Exp_stop();

private:
    /// 实验前预备
    Info prepare_beforeExp();

signals:
    void signal_timeout_forceStop();
};
}  // namespace WIDGET::MAIN

#endif  // WIDGET_MTOP_TOPEXPSTATE_H_1725864233

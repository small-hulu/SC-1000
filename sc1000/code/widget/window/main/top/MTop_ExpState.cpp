#include "MTop_ExpState.h"

#include <QtConcurrent>

#include "communication/COM.hpp"
#include "communication/controller/DispatchController.h"
#include "communication/controller/WatchDog.h"
#include "controller/AppEvent.h"
#include "controller/CONTROLLER_Consumable.h"
#include "controller/CONTROLLER_Sample.h"
#include "controller/device/CDevice.hpp"
#include "global/GLOBAL_DeviceState.h"
#include "widget/dialog/DIALOG.h"

namespace WIDGET::MAIN {

ExpStateController::ExpStateController() {
    connect(&timer_pause_byDevice, &QTimer::timeout, this,
            [this]() { emit signal_timeout_forceStop(); });
}

ExpStateController::~ExpStateController() {
    timer_pause_byDevice.stop();
    lock_pause_byDevice.reset();
    lock_pause_byUser.reset();
}

void ExpStateController::Reset() {
    timer_pause_byDevice.stop();
    lock_pause_byDevice.reset();
    lock_pause_byUser.reset();
    Set_State(ExpState::idle);
    COM::DispatchController::instance().Reset();
}

void ExpStateController::Set_State(ExpStateController::ExpState newState) {
    switch (newState) {
    case ExpState::idle: {
        // pass
    } break;
    case ExpState::running: {
        // pass
    } break;
    case ExpState::pause_byUser: {
        // pass
    } break;
    case ExpState::pause_byDevice: {
        QList<COM::DeviceManager::ownership_type*> flagList;
        for (int i = 0; i < (int)COM::DeviceManager::atomic_flag::Enum_Count; i += 1) {
            flagList << &(COM::DeviceManager::enum_to_owner((COM::DeviceManager::atomic_flag)i));
        }
        lock_pause_byDevice = COM::MultLock::Create_entity(flagList, COM::MultLock::Level_0);
        timer_pause_byDevice.setSingleShot(true);
        timer_pause_byDevice.setInterval(Time_Interval_pause_byDevice);
    } break;
    case ExpState::stop: {
        // pass
    } break;
    default: {
        assert(false);
    }
    }
    QMetaEnum qenum = QMetaEnum::fromType<ExpState>();
    QString   msg   = QString("{state-modify}[%1] -> [%2]")
                      .arg(qenum.key((int)state))
                      .arg(qenum.key((int)newState));
    qInfo() << msg;
    state = newState;
}
//xsw
void ExpStateController::Exp_start() {
    if (COM::ModbusImpl::instance().isOpen() == false) {
        DIALOG::Create_DWarningDialog("设备通信连接异常");
        //return;
    }

    auto                  task    = std::bind(&ExpStateController::prepare_beforeExp, this);
    QFuture<Info>         future  = QtConcurrent::run(task);
    QFutureWatcher<Info>* watcher = new QFutureWatcher<Info>;
    watcher->setFuture(future);
    QObject::connect(watcher, &QFutureWatcher<Info>::finished, [=] {
        const auto info = watcher->result();
        if (info.ok == false) {
            DIALOG::Create_DWarningDialog(info.msg);
            watcher->deleteLater();
            return;
        }

        Set_State(ExpState::running);
        if (info.msg.isEmpty() == false) {
            DIALOG::Create_DWarningDialog(info.msg);
        }
        CONTROLLER::CDevice::DExperiment::Strat();
        CONTROLLER::CDevice::DLightBand::Set_LightBandColor(
            GLOBAL::Get_DeviceStateColor(GLOBAL::MachineState::Running));
        CONTROLLER::AppEvent::instance().signal_expStateHint(true, "实验进行中");
        watcher->deleteLater();
    });
}

void ExpStateController::Exp_pause() {
    if (COM::ModbusImpl::instance().isOpen() == false) {
        DIALOG::Create_DWarningDialog("设备通信连接异常");
        return;
    }

    lock_pause_byDevice.reset();
    lock_pause_byUser.reset();
    /// 暂停要求最顶级的权限
    auto&& owner = COM::DeviceManager::enum_to_owner(COM::DeviceManager::atomic_flag::ReagentBin);
    lock_pause_byUser = COM::MultLock::Create_entity({&owner}, COM::MultLock::Level_0);
    Set_State(ExpState::pause_byUser);
}

void ExpStateController::Exp_stop() {
    if (COM::ModbusImpl::instance().isOpen() == false) {
        DIALOG::Create_DWarningDialog("设备通信连接异常");
        return;
    }

    switch (state) {
    case ExpState::idle: {
        // pass
    } break;
    case ExpState::running: {
        // pass
    } break;
    case ExpState::pause_byUser: {
        lock_pause_byUser.reset();
    } break;
    case ExpState::pause_byDevice: {
        timer_pause_byDevice.stop();
        lock_pause_byDevice.reset();
        COM::WatchDog::instance().Start_thread();
    } break;
    case ExpState::stop: {
        // pass
    } break;
    default: {
        assert(false);
    }
    }

    CONTROLLER::CDevice::DExperiment::Stop();
    Set_State(ExpState::idle);

    using Sample    = CONTROLLER::Sample;
    using ESample   = Sample::entity_type;
    auto sampleList = Sample::instance().Get_isLoadList();
    for (auto&& sample : sampleList) {
        if (sample->table[Sample::entity_attr::state].toInt() ==
            (int)Sample::entity_type::TubeState::Testing) {
            sample->table[Sample::entity_attr::state] =
                QString::number((int)Sample::entity_type::TubeState::Uncompleted);
            sample->Update_toDB();
        }
    }
}

ExpStateController::Info ExpStateController::prepare_beforeExp() {
    Info info;

    switch (state) {
    case ExpState::idle: {
        // pass
    } break;
    case ExpState::running: {
        // pass
    } break;
    case ExpState::pause_byUser: {
        /// 是由暂停重新转为运行的
        /// 因此不用做前置处理
        using Device  = COM::DReagentBin;
        auto&& device = Device::instance();
        device.Set_Config(Device::Mode_Homing, {});
        device.Exec_sequencedTask();

        lock_pause_byUser.reset();
        info.ok = true;
        return info;
    } break;
    case ExpState::pause_byDevice: {
        timer_pause_byDevice.stop();
        lock_pause_byDevice.reset();
        COM::WatchDog::instance().Start_thread();
        info.ok = true;
        return info;
    } break;
    case ExpState::stop: {
        // pass
    } break;
    default: {
        assert(false);
    }
    }

    CONTROLLER::CDevice::DMaintain::Homing_allDevice();

    /// 清洗液xsw
    auto&& wash = CONTROLLER::Consumable::instance().cons_washLiquid;
    auto   rate = wash.Get_ratio();
    if (rate < 0.6) {
        info.msg = QString("清洗液余量不足");
        //return info;
    }

    /// 检测底物AB
    auto&&     A       = CONTROLLER::Consumable::instance().cons_ASubstrat;
    auto&&     B       = CONTROLLER::Consumable::instance().cons_BSubstrat;
    const auto MinRate = ENTITY::EInterface_Consume::Min_Rate;
    if (A.Get_ratio() <= MinRate || B.Get_ratio() <= MinRate) {
        info.msg = QString("激发液余量过低，可能影响实验结果");
    }

    info.ok = true;
    return info;
}

}  // namespace WIDGET::MAIN

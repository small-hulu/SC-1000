#include "WatchDog.h"

#include <QTimer>

#include "../COM_Device.hpp"
#include "../config/SharedMemory.h"
#include "../device/DeviceManager.h"
#include "../device/ModbusImpl.h"
#include "../device/mixing/DMixing.h"
#include "../utility/Helper.h"
#include "../utility/IniConfig.h"
#include "../utility/PDT.h"
#include "DeviceOpAuthorizeCU.h"
#include "Event.h"
#include "communication/bootloader/bootloader_operation.h"
#include "communication/config/CLteErrorCode.hpp"

namespace COM {

WatchDog::WatchDog(QObject* parent) : ICOMThread(parent) {
    setObjectName(__func__);
    Reset();

    qInfo() << objectName() + "Construction complete";
}

void WatchDog::Reset() {
    SharedMemory::instance().flag_autoReagentBinMixingInterruptServicing = false;

    m_watchedTaskList.clear();

    Append_watchTask(std::bind(&WatchDog::check_mcuError, this));
    Append_watchTask(std::bind(&WatchDog::check_consumable, this));

    Append_watchTask(std::bind(&WatchDog::exec_drainage, this));

    auto& ini = IniConfig::instance();
    //自动上料
    if (ini[KI::enable_loopLoadMaterial].toInt()) {
        Append_watchTask(std::bind(&WatchDog::check_loadMaterial, this));
    }
    //试剂仓自动混匀
    if (ini[KI::enable_reagentBinSpinLoop].toInt()) {
        Append_watchTask(std::bind(&WatchDog::exec_reagentBinSpinLoop, this));
    }
}

void WatchDog::Append_watchTask(const WatchDog::SelfTask& task) {
    m_watchedTaskList.push_back(task);
}

void WatchDog::check_mcuError() {
    auto& event        = Event::instance();
    auto  check_device = [&](auto&& device) -> bool {
        if (Helper::can_check(device) == false) {
            /// todo
            return true;
        }

        auto errPkg = device.Check_mcuError();
        if (errPkg.code != CLteErrorCode::ErrorCode::SCNoError) {
            const int deviceAddress = device.get_dataUnitConfig().deviceAddress;
            QString   msg           = device.objectName() + errPkg.errorStr;
            emit      event.signal_mcuError((int)errPkg.code, msg);
            return false;
        }
        return true;
    };

    QList<std::function<bool()>> taskList;
    taskList << [&]() -> bool { return check_device(DReagentBin::instance()); };          /// 10
    taskList << [&]() -> bool { return check_device(DPipetteNeedle::instance()); };       /// 11
    taskList << [&]() -> bool { return check_device(DGripper::instance()); };             /// 12
    taskList << [&]() -> bool { return check_device(DMagneticSeparation::instance()); };  /// 13
    taskList << [&]() -> bool { return check_device(DMixing::instance()); };              /// 14
    taskList << [&]() -> bool { return check_device(DPhotoelectric::instance()); };       /// 15
    taskList << [&]() -> bool { return check_device(DThermostat::instance()); };          /// 16
    for (int i = 0; i < taskList.size(); i += 1) {
        auto&& task = taskList[i];
        if (false == task()) {
            qCritical() << QString("Address%1device check error! WatchDog Stop!").arg(i + 10);
            th_isRunning = false;
            return;
        }
    }
}

void WatchDog::check_consumable() {
    auto& event = Event::instance();

    /// 检测清洗液
    {
        auto&& device = DPipetteNeedle::instance();
        if (Helper::can_check(device) == false) {
            return;
        }

        auto& ini = IniConfig::instance();
        if (ini[KI::enable_fillBottle].toInt() == 1) {
            auto state = device.Get_mcuStateBits();
            /**
             * 清洗液的低位浮子传感器有问题，则加注清洗液
             */
            if (state.u1002.b15_BufferBottleLower == 0) {
                auto& owner =
                    DeviceManager::enum_to_owner(DeviceManager::atomic_flag::PipettingNeedle);
                auto lock = MultLock::Create_entity({&owner});
                device.Set_Config(DPipetteNeedle::Mode_FillBottle, {});
                device.Exec_sequencedTask();
            }

            state = device.Get_mcuStateBits();
            if (state.u1002.b15_BufferBottleLower == 0) {
                th_isRunning = false;
                emit event.signal_consumeWaring("清洗液不足");
                return;
            }
        }
    }

    /// 废液桶不足
    {
        auto&& device = DMixing::instance();
        if (Helper::can_check(device) == false) {
            return;
        }

        auto state = device.Get_mcuStateBits();
        /**
         * 废液桶满了，则报警
         */
        if (state.u1000_Mixing.b08_wasteLiquidTankFull == 1) {
            th_isRunning = false;
            emit event.signal_consumeWaring("废液桶满");
            return;
        }
    }
}

/**
 * 《上料部分》
 * 因为和混匀，分杯用的是同一个板子
 * 因此，需要做互斥
 */
void WatchDog::check_loadMaterial() {
    auto&& device = DMixing::instance();
    if (Helper::can_check(device) == false) {
        return;
    }

    /// + 有效
    /// + 滑道空
    /// + 仓有料 （此处传感器不灵敏）
    DMixing::StateBits state = device.Get_mcuStateBits();
    if ((state.isValid) && (!state.u1000_Mixing.b01_slideFull)
        /*&& (state.u1000_Mixing.b00_hoppeFull) */
    ) {
        device.Set_Config(DMixing::Mode_EnableLoadMaterial);
        device.Exec_sequencedTask();
    }
}

void WatchDog::exec_drainage() {
    static int stepCount  = 0;
    static int stateFlag  = 0;
    stepCount            += Loop_Interval;

    auto set_drainageState = [](bool flag) {
        auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::MagneticSeparation);
        auto  lock  = MultLock::Create_entity({&owner});

        auto&&          device   = DMagneticSeparation::instance();
        QModbusDataUnit dataUnit = device.create_modbusData(1, 1001);
        auto            reply    = device.read(dataUnit);
        dataUnit                 = reply.data;
        const uint16_t oldVal    = dataUnit.value(0);
        qInfo() << QString("set_drainageState oldVal = %1").arg(oldVal);
        dataUnit.setValue(0, set_integerBit(oldVal, 8, flag));
        device.write(dataUnit);
        qInfo() << QString("set_drainageState newVal = %1").arg(set_integerBit(oldVal, 8, flag));
    };

    switch (stateFlag) {
    case 0: {
        constexpr int targetTime = (60 * 1000) * 30;
        if (stepCount >= targetTime) {
            set_drainageState(1);

            stateFlag  = 1;
            stepCount -= targetTime;
        }
    } break;
    case 1: {
        auto&     ini        = IniConfig::instance();
        const int targetTime = ini[KI::time_drainage].toInt();
        if (stepCount >= targetTime) {
            set_drainageState(0);

            stateFlag  = 0;
            stepCount -= targetTime;
        }
    } break;
    }
}

void WatchDog::exec_reagentBinSpinLoop() {
    static int stepCount  = 0;
    stepCount            += Loop_Interval;

    /// 三分钟转一次
    if (stepCount >= 3 * 60 * 1000) {
        stepCount = 0;

        auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::ReagentBin);
        auto  lock  = MultLock::Create_entity({&owner});
        if (DIncubate::instance().Get_CU().is_load == false) {
            DReagentBin::instance().Loop_self();
            SharedMemory::instance().flag_autoReagentBinMixingInterruptServicing = false;
        } else {
            SharedMemory::instance().flag_autoReagentBinMixingInterruptServicing = true;
        }
    }
}

void WatchDog::run() {
    while (th_isRunning) {
        std::unique_lock<std::mutex> lock(th_taskMutex);
        th_taskCV.wait_for(lock, std::chrono::milliseconds(Loop_Interval));
        if (th_isRunning == false) {
            break;
        }
        for (auto&& task : m_watchedTaskList) {
            task();
        }
    }
}
}  // namespace COM

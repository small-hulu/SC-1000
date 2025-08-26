#include "DeviceStateMediator.h"

#include <QMetaEnum>
#include <memory>

#include "../config/CKeyJson.hpp"
#include "../config/SharedMemory.h"
#include "../device/DeviceManager.h"
#include "../device/gripper/DGripper.h"
#include "../device/incubate/DIncubate.h"
#include "../device/magneticSeparation/DMagneticSeparation.h"
#include "../device/mixing/DMixing.h"
#include "../device/photoelectric/DPhotoelectric.h"
#include "../device/reagentBin/DReagentBin.h"
#include "DeviceOpAuthorizeCU.h"
#include "Event.h"

namespace COM {
namespace {
void avoid_Gripper() {
    static int cnt = 0;

    auto& grip = DGripper::instance();
    if (cnt++ % 10 == 0) {
        grip.Set_Config(DGripper::Mode_Homing, {});
    } else {
        grip.Set_Config(DGripper::Mode_XYPosition, {});
    }
    grip.Exec_sequencedTask();
}

void avoid_PipettingNeedle() {}

struct IDeviceMediator {
    virtual ~IDeviceMediator() = default;

    DeviceStateMediator* caller = nullptr;
    Tube::entity_tube    tube;
    IDeviceMediator(DeviceStateMediator* ptr) : caller(ptr) {
        tube = caller->Get_tube();
    }

    using State = DeviceStateMediator::State;
    //from对应器件状态检查
    virtual State check_from_stateCU() {
        return State::Success;
    }
    //from对应器件状态修改
    virtual State modify_from_stateCU() {
        return State::Success;
    }
    //to对应器件状态检查
    virtual State check_to_stateCU() {
        return State::Success;
    }
    //to对应器件状态修改
    virtual State modify_to_stateCU() {
        return State::Success;
    }
};

struct CustomerReagentBin : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    using Device   = DReagentBin;
    Device& device = Device::instance();

    virtual State check_from_stateCU() override {
        auto      op      = caller->Get_json()[KJ::from].toObject();
        const int idxBin  = op[KJ::idxBinGroup].toInt();
        const int idxBack = op[KJ::idxBinBack].toInt();
        auto      params  = device.GetInfo_backHole(idxBack);
        device.Offset_byIndex(params, idxBin);
        device.Offset_byProximity(params);
        device.Set_Config(Device::Mode_Position, params);
        device.Exec_sequencedTask();

        return State::Success;
    }
};

struct CustomerMagneticSeparation : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    using Device   = DMagneticSeparation;
    Device& device = Device::instance();

    virtual State check_from_stateCU() {
        int idx = tube->paramPkg.magneticSeparationIndex;

        auto params = device.GetParams_to_Grapper();
        device.Offset_byIndex(params, idx);
        device.Offset_byProximity(params);
        device.Set_Config(Device::Mode_AbleTrayMove, params);
        device.Exec_sequencedTask();
        return State::Success;
    }

    virtual State modify_from_stateCU() {
        auto& cu = device.Get_CU();
        cu.Affirm_fetch(tube);
        return State::Success;
    }

    virtual State check_to_stateCU() {
        auto& cu  = device.Get_CU();
        int   idx = cu.Request_putIn();
        if (idx < 0) {
            return State::Again;
        }
        tube->paramPkg.magneticSeparationIndex = idx;

        auto params = device.GetParams_to_Grapper();
        device.Offset_byIndex(params, idx);
        device.Offset_byProximity(params);
        device.Set_Config(Device::Mode_AbleTrayMove, params);
        device.Exec_sequencedTask();
        return State::Success;
    }

    virtual State modify_to_stateCU() {
        auto& cu = device.Get_CU();
        cu.Affirm_putIn(tube);
        return State::Success;
    }
};

struct CustomerMixing : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    using Device   = DMixing;
    Device& device = Device::instance();

    virtual State modify_from_stateCU() {
        auto& cu = device.Get_CU();
        cu.Set_mixingOccupy(false);
        return State::Success;
    }
    virtual State check_to_stateCU() {
        auto& cu = device.Get_CU();
        if (cu.Is_mixingOccupy()) {
            return State::Again;
        }
        return State::Success;
    }
    virtual State modify_to_stateCU() {
        auto& cu = device.Get_CU();
        cu.Set_mixingOccupy(true);
        device.Set_Config(Device::Mode_EnableMix);
        device.Exec_sequencedTask();

        //! 防止移液针的撞击
        //! 并非所有机器都需要，但是统一
        auto atomic_task = []() -> void {
            auto&            grip     = DGripper::instance();
            DGripper::Params gparams  = grip.Get_mcuParams();
            gparams.yPosition        -= 1000;
            grip.Set_Config(DGripper::Mode_XYPosition, gparams);
            grip.Exec_sequencedTask();
        };

        if (DeviceManager::Analyze_flagList(caller->Get_json())
                .contains(DeviceManager::atomic_flag::Gripper)) {
            atomic_task();
        } else {
            auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::Gripper);
            auto  lock  = MultLock::Create_entity({&owner});
            atomic_task();
        }

        return State::Success;
    }
};

struct CustomerPhotoelectric : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    using Device   = DPhotoelectric;
    Device& device = Device::instance();

    virtual State check_from_stateCU() {
        device.Set_Config(Device::Mode_DoorOpen, {});
        device.Exec_sequencedTask();
        return State::Success;
    }
    virtual State modify_from_stateCU() {
        device.Set_Config(Device::Mode_DoorClose, {});
        device.Exec_sequencedTask();
        auto& cu = device.Get_CU();
        cu.Set_occupy(false);
        return State::Success;
    }
    virtual State check_to_stateCU() {
        auto& cu = device.Get_CU();
        if (cu.Is_occupy() == true) {
            return State::Again;
        }
        device.Set_Config(Device::Mode_DoorOpen, {});
        device.Exec_sequencedTask();
        return State::Success;
    }
    virtual State modify_to_stateCU() {
        auto& cu = device.Get_CU();
        cu.Set_occupy(true);

        device.Set_Config(Device::Mode_DoorClose, {});
        device.Exec_sequencedTask();

        auto params = device.GetParams_defaultCapacity();
        auto op     = caller->Get_json()[KJ::to].toObject();
        if (auto sub = caller->Get_json()[KJ::to].toObject()[KJ::capacity]; sub.isDouble()) {
            params.capacity = sub.toInt();
        }
        device.Set_Config(Device::Mode_Detect, params);
        device.Exec_sequencedTask();
        tube->Set_expValue(device.Get_value());

        return State::Success;
    }
};

struct CustomerIncubate : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    using Device   = DIncubate;
    Device& device = Device::instance();

    virtual State modify_from_stateCU() {
        auto& cu = device.Get_CU();
        cu.Affirm_fetch(tube);
        return State::Success;
    }
    virtual State check_to_stateCU() {
        auto op = caller->Get_json()[KJ::to].toObject();

        //申请一个孔位
        auto& cu  = device.Get_CU();
        auto  pii = cu.Request_toPutIn();
        if (pii.first == -1 || pii.second == -1) {
            return State::Again;
        }

        tube->paramPkg.incubateX = pii.first;
        tube->paramPkg.incubateY = pii.second;
        tube->paramPkg.sleepTime = op[KJ::sleepTime].toInt();

        return State::Success;
    }
    virtual State modify_to_stateCU() {
        auto& cu = device.Get_CU();
        cu.Affirm_toPutIn(tube);
        return State::Success;
    }
};

struct CustomerCup : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    using Device   = DMixing;
    Device& device = Device::instance();

    virtual State check_from_stateCU() {
        /// 进行20次尝试
        constexpr int tryCount = 20;
        for (int i = 0; i < tryCount; i += 1) {
            auto state = device.Get_mcuStateBits();
            /// 分杯位有杯，直接使用
            if (state.u1000_Mixing.b02_cupholeFull) {
                break;
            }
            /// 否则推杯一下
            else {
                device.Set_Config(Device::Mode_EnableDivideCup);
                device.Exec_sequencedTask();
                state = device.Get_mcuStateBits();
                if (state.u1000_Mixing.b02_cupholeFull) {
                    break;
                }
            }
            QThread::sleep(1);

            /// 滑道不足，尝试上料一次
            if (state.u1000_Mixing.b01_slideFull == 0) {
                device.Set_Config(Device::Mode_EnableLoadMaterial);
                device.Exec_sequencedTask();
            }

            if (i >= tryCount / 2) {
                Event::instance().signal_waring(QString("%1次 获取反应杯失败").arg(i));
            }
        }

        auto state = device.Get_mcuStateBits();
        if (state.u1000_Mixing.b02_cupholeFull) {
            return State::Success;
        }

        qCritical() << "ForceStop 多次获取反应杯失败" << state.to_string();
        /// 强制停止
        return State::ForceStop;
    }

    //! 上杯操作转移后，夹爪避让
    //! 最初版是homing, 但是影响速度
    //! 现在改为直接到 (0, 0) 位置
    virtual State modify_from_stateCU() {
        if (DeviceManager::Analyze_flagList(caller->Get_json())
                .contains(DeviceManager::atomic_flag::Gripper)) {
            avoid_Gripper();
        } else {
            auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::Gripper);
            auto  lock  = MultLock::Create_entity({&owner});
            avoid_Gripper();
        }

        return State::Success;
    }
};

struct CustomerLoad : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    using Device   = DIncubate;
    Device& device = Device::instance();

    virtual State modify_from_stateCU() {
        auto& cu   = device.Get_CU();
        cu.is_load = false;
        return State::Success;
    }
    virtual State check_to_stateCU() {
        /// 如果是移液针状态的话，默认成功
        const QString name = caller->Get_json()[KJ::deviceName].toString();
        if (name == KJ::PipettingNeedle || name == KJ::DoubleSuction) {
            return State::Success;
        }

        auto& cu = device.Get_CU();
        /// 上料位是否有杯子
        /// 有杯子，无效=>重试
        if (cu.is_load) {
            return State::Again;
        }
        /// 没有杯子
        /// 需要判断是否需要混匀
        else {
            if (SharedMemory::instance().flag_autoReagentBinMixingInterruptServicing == true) {
                DReagentBin::instance().Loop_self();
                SharedMemory::instance().flag_autoReagentBinMixingInterruptServicing = false;
            }
        }
        return State::Success;
    }
    virtual State modify_to_stateCU() {
        auto& cu   = device.Get_CU();
        cu.is_load = true;

        if (DeviceManager::Analyze_flagList(caller->Get_json())
                .contains(DeviceManager::atomic_flag::Gripper)) {
            avoid_Gripper();
        } else {
            auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::Gripper);
            auto  lock  = MultLock::Create_entity({&owner});
            avoid_Gripper();
        }

        return State::Success;
    }
};

struct CustomerDilute : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    using Device   = DIncubate;
    Device& device = Device::instance();

    virtual State modify_from_stateCU() {
        /// 如果是移液针状态的话，默认成功
        const QString name = caller->Get_json()[KJ::deviceName].toString();
        if (name == KJ::PipettingNeedle) {
            return State::Success;
        }

        auto& cu     = device.Get_CU();
        cu.is_dilute = false;
        return State::Success;
    }
    virtual State check_to_stateCU() {
        /// 如果是移液针状态的话，默认成功
        const QString name = caller->Get_json()[KJ::deviceName].toString();
        if (name == KJ::PipettingNeedle) {
            return State::Success;
        }

        /// 夹爪状态
        auto& cu = device.Get_CU();
        if (cu.is_dilute) {
            return State::Again;
        }
        return State::Success;
    }
    virtual State modify_to_stateCU() {
        auto& cu     = device.Get_CU();
        cu.is_dilute = true;

        if (DeviceManager::Analyze_flagList(caller->Get_json())
                .contains(DeviceManager::atomic_flag::Gripper)) {
            avoid_Gripper();
        } else {
            auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::Gripper);
            auto  lock  = MultLock::Create_entity({&owner});
            avoid_Gripper();
        }

        return State::Success;
    }
};

struct CustomerThrow : IDeviceMediator {
    using IDeviceMediator::IDeviceMediator;
    virtual State modify_from_stateCU() {
        DGripper::instance().Set_ZPositionHeightFlag(true);
        return State::Success;
    }

    virtual State modify_to_stateCU() {
        DGripper::instance().Set_ZPositionHeightFlag(false);
        return State::Success;
    }
};

std::unique_ptr<IDeviceMediator> Create_MediatorCustomer(DeviceStateMediator*       ptr,
                                                         DeviceManager::atomic_flag deviceFlag) {
    using Enum    = DeviceManager::atomic_flag;
    using ret_ptr = std::unique_ptr<IDeviceMediator>;
    switch (deviceFlag) {
    case Enum::UnKnow            : return {};
    case Enum::Gripper           : return {};
    case Enum::PipettingNeedle   : return {};
    case Enum::ReagentBin        : return ret_ptr(new CustomerReagentBin{ptr});
    case Enum::MagneticSeparation: return ret_ptr(new CustomerMagneticSeparation{ptr});
    case Enum::Mixing            : return ret_ptr(new CustomerMixing{ptr});
    case Enum::Photoelectric     : return ret_ptr(new CustomerPhotoelectric{ptr});
    case Enum::Incubate          : return ret_ptr(new CustomerIncubate{ptr});
    case Enum::Cup               : return ret_ptr(new CustomerCup{ptr});
    case Enum::Load              : return ret_ptr(new CustomerLoad{ptr});
    case Enum::Dilute            : return ret_ptr(new CustomerDilute{ptr});
    case Enum::Throw             : return ret_ptr(new CustomerThrow{ptr});
    case Enum::Enum_Count        : return {};
    default                      : return {};
    }
    return {};
}

}  // namespace

DeviceStateMediator::DeviceStateMediator(const QJsonObject& json, Tube::entity_tube tube,
                                         QObject* parent)
    : QObject(parent) {
    m_json = json;
    m_tube = tube;

    /// 该模式只针对 minor 类型操作来处理 找出from to的对象名称
    if (json[KJ::actionType].toString() == KJ::ActionMinor) {
        QString from = json[KJ::from].toObject()[KJ::target].toString();
        m_fromType   = DeviceManager::string_to_enum(from);
        QString to   = json[KJ::to].toObject()[KJ::target].toString();
        m_toType     = DeviceManager::string_to_enum(to);
    }
}

DeviceStateMediator::State DeviceStateMediator::Check_stateCU() {
    const auto fromState = check_from_stateCU();
    const auto toState   = check_to_stateCU();

    if (fromState == ForceStop || toState == ForceStop) {
        return ForceStop;
    }
    if (fromState != Success || toState != Success) {
        QMetaEnum qenum = QMetaEnum::fromType<State>();
        qDebug() << QString("(Check_stateCU)from:[%1], to[%2]")  //
                        .arg(qenum.key(fromState))               //
                        .arg(qenum.key(toState));
        return Again;
    }
    return Success;
}

DeviceStateMediator::State DeviceStateMediator::Modify_stateCU() {
    const auto fromState = modify_from_stateCU();
    const auto toState   = modify_to_stateCU();

    if (fromState == ForceStop || toState == ForceStop) {
        return ForceStop;
    }
    if (fromState != Success || toState != Success) {
        QMetaEnum qenum = QMetaEnum::fromType<State>();
        qDebug() << QString("(Modify_stateCU)from:[%1], to[%2]")  //
                        .arg(qenum.key(fromState))                //
                        .arg(qenum.key(toState));
        return Again;
    }
    return Success;
}

/** from begin */
DeviceStateMediator::State DeviceStateMediator::check_from_stateCU() {
    auto customer = Create_MediatorCustomer(this, m_fromType);
    if (customer == nullptr) {
        qWarning() << __PRETTY_FUNCTION__ << "解析为空，让运行";
        return Success;
    }
    return customer->check_from_stateCU();
}

DeviceStateMediator::State DeviceStateMediator::modify_from_stateCU() {
    auto customer = Create_MediatorCustomer(this, m_fromType);
    if (customer == nullptr) {
        qWarning() << __PRETTY_FUNCTION__ << "解析为空，让运行";
        return Success;
    }
    return customer->modify_from_stateCU();
}
/** from end */

/** to begin */
DeviceStateMediator::State DeviceStateMediator::check_to_stateCU() {
    auto customer = Create_MediatorCustomer(this, m_toType);
    if (customer == nullptr) {
        qWarning() << __PRETTY_FUNCTION__ << "解析为空，让运行";
        return Success;
    }
    return customer->check_to_stateCU();
}

DeviceStateMediator::State DeviceStateMediator::modify_to_stateCU() {
    auto customer = Create_MediatorCustomer(this, m_toType);
    if (customer == nullptr) {
        qWarning() << __PRETTY_FUNCTION__ << "解析为空，让运行";
        return Success;
    }
    return customer->modify_to_stateCU();
}
/** to end */

}  // namespace COM

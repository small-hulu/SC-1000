#include "CUMagneticSeparation.h"

#include <QEventLoop>
#include <QTimer>
#include <mutex>

#include "../../controller/DeviceOpAuthorizeCU.h"
#include "../../utility/IniConfig.h"
#include "DMagneticSeparation.h"
#include "log/LOG_SLog.h"

#define ON_DEBUG_MODBUS (0)

#define DEBUG_FILEUNIT()    \
    if (!ON_DEBUG_MODBUS) { \
    } else                  \
        qDebug()

namespace COM {

CUMagneticSeparation::CUMagneticSeparation() {
    setObjectName("磁分离自旋单元");
    auto&& ini = IniConfig::instance();
    ms         = ini[KI::magneticSeparation_spinTime].toInt();
    Reset();
}

QString CUMagneticSeparation::to_string() const {
    QString msg;
    msg += border.to_string() + "\n";
    for (const Cell& cell : table) {
        msg += cell.to_string() + "\n";
    }
    return msg;
}

/**
 * 表默认初始化长度为6
 * 并初始化这几个位置的类型和状态
 */
void CUMagneticSeparation::Reset() {
    for (int i = 0; i < table.size(); i += 1) {
        if (table[i].state != State::Unoccupy) {
            qWarning() << QString("磁分离重置 idx[%1]").arg(i) << table[i].to_string();
        }
    }
    table.clear();
    table.resize(Cell_Count);

    /// [0, 12)
    /// 逻辑上的递减
    for (int i = 0; i < table.size(); i += 1) {
        table[i].idxLogic   = offset(border.inBorder - i);
        table[i].idxReality = offset(border.inBorder + i);
        table[i].state      = State::Unoccupy;
        table[i].tube       = {};
    }
}

/**
 * 请求放入
 */
int CUMagneticSeparation::Request_putIn() {
    /// 1. 保证只在外部
    for (int i = 2; i <= Cell_Count - border.get_borderLength(); i += 1) {
        /// 向前查找
        int     idx  = offset(border.inBorder - i);
        CellRef cell = get_logicCell(idx);
        /// 无管，就选定是这个了
        if (cell.tube == nullptr) {
            return cell.idxReality;
        }
    }

    return -1;
}

/**
 * 确认放入
 * 检验判断和可放入的点是否相等
 */
void CUMagneticSeparation::Affirm_putIn(Tube::entity_tube tube) {
    exInvolve = true;

    tube->Set_state(Tube::State_InMagneticSeparation);
    int     idx  = tube->paramPkg.magneticSeparationIndex;
    CellRef cell = get_RealityCell(idx);
    cell.tube    = tube;
    cell.state   = State::NeedNeedle0;

    using Device          = DMagneticSeparation;
    auto&          device = Device::instance();
    Device::Params params;

    params = device.GetParams_to_Wash();
    device.Offset_byIndex(params, cell.idxReality);
    device.Offset_byProximity(params);
    device.Set_Config(Device::Mode_AbleTrayMove, params);
    device.Exec_sequencedTask();
    device.Set_Config(Device::Mode_AbleWash, params);
    device.Exec_sequencedTask();

    align_Logictable();
}

/**
 * 直接出队
 */
void CUMagneticSeparation::Affirm_fetch(Tube::entity_tube tube) {
    exInvolve = true;

    int     idx  = tube->paramPkg.magneticSeparationIndex;
    CellRef cell = get_RealityCell(idx);
    cell.tube    = {};
    cell.state   = State::Unoccupy;

    align_Logictable();
}

/**
 * 移动只要根据逻辑编号到物理编号即可
 */
void CUMagneticSeparation::align_Logictable(const char* const func) {
    auto needMove = [this]() -> bool {
        for (CellRef cell : table) {
            if (cell.tube && cell.state != State::CanTransferOut) {
                return true;
            }
        }
        return false;
    };
    if (needMove() == false) {
        return;
    }

    auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::MagneticSeparation);
    MultLock::entity_multLock lock;
    if (owner == false) {
        DEBUG_FILEUNIT() << QThread::currentThread() << "not has 锁状态进入" << func;
        lock = MultLock::Create_entity({&owner});
    } else {
        DEBUG_FILEUNIT() << QThread::currentThread() << "has 锁状态进入" << func;
    }

    //! 移动只要根据逻辑编号到物理编号即可
    for (CellRef cell : table) {
        /// 无论这个cell有没有管，作为映射都是可以的
        if (cell.idxLogic == border.needle0) {
            using Device          = DMagneticSeparation;
            auto&          device = Device::instance();
            Device::Params params;
            params = device.GetParams_to_Needle0();
            device.Offset_byIndex(params, cell.idxReality);
            device.Offset_byProximity(params);
            device.Set_Config(Device::Mode_AbleTrayMove, params);
            device.Exec_sequencedTask();
        }
    }

    DEBUG_FILEUNIT() << QThread::currentThread() << " fun-out " << func;
}

/**
 * 自旋
 * ==============================
 * 先移动，后执行
 */
void CUMagneticSeparation::poll_spin() {
    //! (判断)
    /// * 是否需要步进一次
    /// * 条件：有管 & 不是需要被转移走的
    auto needMove = [this]() -> bool {
        for (CellRef cell : table) {
            if (cell.tube && cell.state != State::CanTransferOut) {
                return true;
            }
        }
        return false;
    };

    //! (处理)
    /// * 逻辑编号都+1
    /// * 转盘前进一步
    auto moveOneStep = [this]() -> void {
        //! 逻辑编号+1
        /// 这里的±1是根据旋转顺逆而定的
        for (CellRef cell : table) {
            cell.idxLogic = offset(cell.idxLogic + 1);
        }
    };

    /** ****************************************************
     * 整体性质处理
     * 1. 判断是否需要旋转
     * 2. 判断是否需要进行排液
     * 3. 遍历每个管，走进行状态机判断
     *******************************************************/
    //! 是否需要步进，并执行步进
    if (needMove() == false) {
        // pass
    } else {
        moveOneStep();
        align_Logictable();
    }
}

void CUMagneticSeparation::poll_state() {
    //! (判断 + 处理)
    /// * 是否需要排液针排液
    auto checkDrainNeedle = [this]() -> void {
        bool flag = false;
        for (CellRef cell : table) {
            if (cell.tube == nullptr) {
                continue;
            }
            flag |= (cell.state == State::NeedNeedle0) && (cell.idxLogic == border.needle0);
            flag |= (cell.state == State::NeedNeedle1) && (cell.idxLogic == border.needle1);
            flag |= (cell.state == State::NeedNeedle2) && (cell.idxLogic == border.needle2);
        }

        if (flag) {
            using Device = DMagneticSeparation;
            auto& device = Device::instance();
            device.Set_Config(Device::Mode_AbleDrainNeedle, {});
            device.Exec_sequencedTask();
        }
    };
    checkDrainNeedle();

    using Device          = DMagneticSeparation;
    auto&          device = Device::instance();
    Device::Params params;

    //! 直接走状态机
    /// * 注意：此处是排液完成了的
    for (CellRef cell : table) {
        auto& owner = DeviceManager::enum_to_owner(DeviceManager::atomic_flag::MagneticSeparation);
        auto  lock  = MultLock::Create_entity({&owner});
        if (cell.tube == nullptr) {
            continue;
        }

        DEBUG_FILEUNIT() << cell.to_string();
        switch (cell.state) {
        case State::Unoccupy: {
            qWarning() << "磁分离自旋 Unoccupy 状态错误";
        } break;
        case State::NeedNeedle0: {
            if (cell.idxLogic != border.needle0) {
                break;
            }
            params = device.GetParams_to_Wash();
            device.Offset_byIndex(params, cell.idxReality);
            device.Offset_byProximity(params);
            device.Set_Config(Device::Mode_AbleTrayMove, params);
            device.Exec_sequencedTask();
            device.Set_Config(Device::Mode_AbleWash, params);
            device.Exec_sequencedTask();
            cell.state = State::NeedNeedle1;
        } break;
        case State::NeedNeedle1: {
            if (cell.idxLogic != border.needle1) {
                break;
            }
            params = device.GetParams_to_Wash();
            device.Offset_byIndex(params, cell.idxReality);
            device.Offset_byProximity(params);
            device.Set_Config(Device::Mode_AbleTrayMove, params);
            device.Exec_sequencedTask();
            device.Set_Config(Device::Mode_AbleWash, params);
            device.Exec_sequencedTask();
            cell.state = State::NeedNeedle2;
        } break;
        case State::NeedNeedle2: {
            if (cell.idxLogic != border.needle2) {
                break;
            }
            cell.state = State::NeedASubstrate;

            params = device.GetParams_to_ASubstrate();
            device.Offset_byIndex(params, cell.idxReality);
            device.Offset_byProximity(params);
            device.Set_Config(Device::Mode_AbleTrayMove, params);
            device.Exec_sequencedTask();
            device.Set_Config(Device::Mode_AbleASubstratePump, params);
            device.Exec_sequencedTask();
            cell.state = State::CanTransferOut;

            cell.tube->Set_state(Tube::State::State_CanBeTransfer);
        } break;
        case State::NeedASubstrate: {
            params = device.GetParams_to_ASubstrate();
            device.Offset_byIndex(params, cell.idxReality);
            device.Offset_byProximity(params);
            device.Set_Config(Device::Mode_AbleTrayMove, params);
            device.Exec_sequencedTask();
            device.Set_Config(Device::Mode_AbleASubstratePump, params);
            device.Exec_sequencedTask();
            cell.state = State::CanTransferOut;

            cell.tube->Set_state(Tube::State::State_CanBeTransfer);
        } break;
        case State::CanTransferOut: {
        } break;
        default: {
            qWarning() << "磁分离自旋 default 状态错误";
        } break;
        }
    }
}

void CUMagneticSeparation::run() {
    /// 因为不一定保证用户设置的 ms 是 >=1000 的整千数
    constexpr int timeInterval = 1000;
    int           cycleCnt     = 0;
    while (th_isRunning) {
        std::unique_lock<std::mutex> lock(th_taskMutex);
        th_taskCV.wait_for(lock, std::chrono::milliseconds(timeInterval));
        if (th_isRunning == false) {
            break;
        }
        cycleCnt += timeInterval;

        bool flag = false;
        {
            auto& owner =
                DeviceManager::enum_to_owner(DeviceManager::atomic_flag::MagneticSeparation);
            auto lock = MultLock::Create_entity({&owner});
            if (exInvolve == true) {
                exInvolve = false;
                cycleCnt  = 0;
                DEBUG_FILEUNIT() << QString("外部介入自旋");
                continue;
            } else if (cycleCnt < ms) {
                continue;
            } else {
                flag = true;
            }
        }
        if (flag) {
            DEBUG_FILEUNIT() << to_string();
            poll_state();

            auto& owner =
                DeviceManager::enum_to_owner(DeviceManager::atomic_flag::MagneticSeparation);
            auto lock = MultLock::Create_entity({&owner});
            poll_spin();
            cycleCnt  = 0;
            exInvolve = false;
        }
    }

    qInfo() << "磁分离自旋 run-end";
}

}  // namespace COM

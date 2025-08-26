#include "DispatchController.h"

#include <QDebug>
#include <QMetaEnum>

#include "Event.h"
#include "communication/COM_Device.hpp"

namespace COM {
DispatchController::DispatchController() {
    Reset();
    m_state = State::Leisure;
}

QString DispatchController::to_string() const {
    QString msg;
    QDebug  qos(&msg);
    qos << " 全任务数量" << m_taskSet.size() << "\n";
    QMetaEnum qenum = QMetaEnum::fromType<State>();
    qos << "当前状态" << qenum.key((int)m_state);
    return msg;
}

void DispatchController::Reset() {
    /// 磁分离状态重置，每个孔置空
    DMagneticSeparation::instance().Get_CU().Reset();
    /// 温裕盘所有孔置空，两个特殊位置（上料位和稀释位）的独立标记也置空
    DIncubate::instance().Get_CU().Reset_cacheFixtureParams();
    /// 光电的孔位也置空
    DPhotoelectric::instance().Get_CU().Reset();

    /// 任务维护
    m_taskSet.clear();
}

TaskPackage::smart_entity DispatchController::Append_taskPackage(int                key,
                                                                 const QJsonObject& ojson) {
    if (m_taskSet.count(key)) {
        return {};
    }

    auto entity    = TaskPackage::Create_entity(ojson);
    m_taskSet[key] = entity;

    connect(entity.get(), &TaskPackage::signal_state, this, [key, this, entity](int state) {
        qInfo() << entity.get() << "单个任务" << TaskPackage::State(state);
        switch (TaskPackage::State(state)) {
        case TaskPackage::State::Invalid: {
        } break;
        case TaskPackage::State::ParsingError: {
        } break;
        case TaskPackage::State::Running: {
        } break;
        case TaskPackage::State::Pause: {
        } break;
        case TaskPackage::State::Stop: {
            Remove_taskPackage(key);
        } break;
        case TaskPackage::State::ForceStop: {
            qWarning() << "全流程异常，强制终止";
            for (auto&& [key, taskPkg] : m_taskSet) {
                Q_UNUSED(key);
                taskPkg->Pause();
            }
            m_state = State::ForceStop;
            emit signal_ForceStop();
        } break;
        case TaskPackage::State::Accomplish: {
            Remove_taskPackage(key);
        } break;
        }

        /// 所有任务全部消除了，则整体也就完成了
        if (m_taskSet.size() == 0) {
            m_state = State::Accomplish;
            emit signal_Accomplish();
        }
    });
    return entity;
}

bool DispatchController::Remove_taskPackage(int key) {
    return m_taskSet.erase(key);
}

DispatchController::State DispatchController::Stop_dispatch() {
    if (m_state != State::Running) {
        return m_state;
    }
    for (auto&& [key, taskPkg] : m_taskSet) {
        Q_UNUSED(key);
        taskPkg->Stop();
    }
    m_state = State::Stop;
    return m_state;
}

DispatchController::State DispatchController::Pause_dispatch() {
    if (m_state != State::Running) {
        return m_state;
    }
    /// 设置任务停止后，taskPkg's state 会立刻变为 State::Pause
    for (auto&& [key, taskPkg] : m_taskSet) {
        Q_UNUSED(key);
        taskPkg->Pause();
    }
    m_state = State::Pause;
    return m_state;
}

DispatchController::State DispatchController::Start_dispatch() {
    /// 空任务默认直接完成
    if (m_taskSet.size() == 0) {
        QTimer::singleShot(0, this, [this]() { emit signal_Accomplish(); });
        return m_state;
    }

    /// 无论是暂停还是重新开始，taskPkg'start 都会自己处理
    for (auto&& [key, taskPkg] : m_taskSet) {
        Q_UNUSED(key);
        if (taskPkg->isRunning() == false) {
            taskPkg->Start();
        }
    }

    m_state = State::Running;
    return m_state;
}
}  // namespace COM

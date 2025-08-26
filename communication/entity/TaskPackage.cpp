#include "TaskPackage.h"

#include <QMetaEnum>

#include "../config/CKeyJson.hpp"
#include "../controller/ActionMajor.h"
#include "../controller/ActionMinor.h"
#include "../controller/ActionPatch.h"
#include "../controller/DeviceOpAuthorizeCU.h"
#include "../controller/DeviceStateMediator.h"
#include "../controller/Parse.h"
#include "../utility/IniConfig.h"
#include "../utility/UnitConv.h"

namespace COM {
TaskPackage::TaskPackage() {
    Reset();
}

TaskPackage::TaskPackage(QJsonObject json) : TaskPackage() {
    m_ojson = std::move(json);
    assert(m_ojson[KJ::processSteps].isArray());
    m_stepsList = m_ojson[KJ::processSteps].toArray();
}

TaskPackage::~TaskPackage() {
    qInfo() << this << QString("~QThread(%1)start").arg(__func__);
    stop_thread();
    qInfo() << this << QString("~QThread(%1)end").arg(__func__);
}

void TaskPackage::Reset() {
    m_state     = State::Invalid;
    m_taskIndex = -1;
    m_tube      = Tube::Create_new_SmartPipe();
    connect(m_tube.get(), &Tube::signal_valueReceived, this, [this](int val) {
        double rate = IniConfig::instance()[KI::photoelectric_valRate].toDouble();
        qDebug()<<"系数"<<rate<<endl;
        qDebug()<<"val"<<val * rate<<endl;
        emit   signal_valueReceived(val * rate);
    });
    /// 将步进转为微升
    connect(m_tube.get(), &Tube::signal_pipetteNeedleSuction, this,
            [this](int idxBinGroup, int idxBinBack, int val) {
                val = motorStep_to_microlitre_PipetteNeedle(val);
                emit signal_pipetteNeedleSuction(idxBinGroup, idxBinBack, val);
            });
}

void TaskPackage::Modify_processStepByIndex(int idx, QJsonObject json) {
    std::lock_guard<std::mutex> lock(m_jsonMtx);
    m_stepsList[idx] = json;
}

TaskPackage::Info TaskPackage::Get_Info() const {
    Info info;
    info.expValue = m_tube->Get_expValue();
    return info;
}

/**
 * 先确保上一轮的操作是结束的
 * 正常首次启动，是从 Invalid -> Running
 * 但可能会出现暂停的情况 Pause
 * ==================================
 * 如果是暂停的
 * 在 stop_thread 中不会修改 state
 * 否则一般来说是处于 Invalid 的
 */
TaskPackage::State TaskPackage::Start() {
    QMetaEnum qenum = QMetaEnum::fromType<State>();
    // qDebug() << this << qenum.key(m_state.load());
    if (m_state == State::Invalid) {
        m_taskIndex = 0;
        m_state     = State::Running;
    } else if (m_state == State::Pause) {
        /// taskIndex 保持原状
        /// state     变为运行
        m_state = State::Running;
    } else if (m_state == State::Running) {
        qInfo() << "继续运行" << qenum.key((int)m_state.load());
        return m_state.load();
    } else {
        qWarning() << "任务启动状态错乱" << qenum.key((int)m_state.load());
        return m_state.load();
    }

    this->start();
    return m_state.load();
}

/**
 * 修改state后run会在下轮自动退出
 */
TaskPackage::State TaskPackage::Pause() {
    m_state = State::Pause;
    return m_state.load();
}

/**
 * 修改state后run会在下轮自动退出
 */
TaskPackage::State TaskPackage::Stop() {
    m_state = State::Stop;
    return m_state.load();
}

void TaskPackage::stop_thread() {
    if (this->isRunning()) {
        //! 变为无效状态，run 会自动退出
        m_state = State::Invalid;
        this->quit();
        this->wait();
    }
}

void TaskPackage::run() {
    QMetaEnum qenum = QMetaEnum::fromType<State>();
    qInfo() << this
            << QString("&&&TaskPackage::RunStart state(%1)").arg(qenum.key((int)m_state.load()));

    /// 优先级的基准
    /// 主要是为了让将急诊项目的优先级提高
    const int basePriority = m_ojson[KJ::emergency].toInt();
    while (State::Running == m_state) {
        QThread::yieldCurrentThread();

        //! [tube-can be transfer]
        if (m_tube->Get_state() != Tube::State_CanBeTransfer) {
            continue;
        }
        //! [tube-can be transfer]

        QJsonObject json = [this]() {
            std::lock_guard<std::mutex> lock(m_jsonMtx);
            return m_stepsList[m_taskIndex].toObject();
        }();
        qDebug().noquote().nospace() << ">Task Node Json>" << json;

        //! [acquire - mutex]
        /// MultLock-raii

        auto level = MultLock::Resolve_Priority(json, basePriority + m_taskIndex);

        //卡在里面的Create_entity,当申请到资源以后就会往下执行   ！！！
        auto lock  = MultLock::Create_entity(json, level);
        //lock为局部变量，在本次循环结束后析构掉，同时解锁资源

        //! [acquire - mutex]

        //! [check - resource state]
        /// 这里的中介者不是为了RAII，而是为了封装
        DeviceStateMediator stateMediator(json, m_tube);
        //node执行前的准备，检查from和to的设备状态，如果状态不对则做相应的处理
        auto                checkState = stateMediator.Check_stateCU();

        /// 无法继续实验，强制实验终止
        if (DeviceStateMediator::State::ForceStop == checkState) {
            m_state = State::ForceStop;
            qWarning() << json << "实验前置动作异常" << checkState;
            break;
        } else if (DeviceStateMediator::State::Success != checkState) {
            /// 前置动作异常，需重试
            continue;
        }
        //! [check - resource state]

        //! [operator action]
        {
            bool     ok       = true;
            TaskNode taskNode = Parse::json_to_task(json, ok);
            if (ok == false) {
                qWarning() << RetCode::TextParsingError << "%文本配置解析失败%" << json;
                m_state = State::ParsingError;
                break;
            }
            const auto ret = taskNode(m_tube);

            auto checkError = [](RetCode code) -> bool {
                switch (code.result) {
                    case RetCode::ForceStop:
                    case RetCode::RefuseError:
                    case RetCode::ItemExistError:
                    case RetCode::UnknownError  : return false;
                    default                     :;
                }
                return true;
            };
            if (checkError(ret) == false) {
                qWarning() << "实验核心动作异常" << ret;
                m_state = State::ForceStop;
                break;
            }

            auto checkWarn = [](RetCode code) -> bool {
                switch (code.result) {
                case RetCode::Insufficient_in:
                case RetCode::Insufficient_out: return false;
                default                       :;
                }
                return true;
            };
            if (checkWarn(ret) == false) {
                qWarning() << "实验核心动作警告" << ret;
                emit signal_waring(ret.to_string());
            }
        }
        //! [operator action]

        //! [modify - resource state]
        /// 手动调用信息
        stateMediator.Modify_stateCU();
        //! [modify - resource state]

        //! [release - mutex]
        /// RAII
        //! [release - mutex]

        //! [ready for next node]
        m_taskIndex += 1;
        //! [ready for next node]

        if (m_taskIndex >= m_stepsList.size()) {
            m_state = State::Accomplish;
        }
    }

    qInfo() << this
            << QString("TaskPackage::RunEnd state(%1) index(%2)")
                   .arg(qenum.key((int)m_state.load()))
                   .arg(m_taskIndex);
    emit signal_state((int)m_state.load());
}

}  // namespace COM

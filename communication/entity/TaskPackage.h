#ifndef TASKPACKAGE_H_1718677506
#define TASKPACKAGE_H_1718677506

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QThread>
#include <thread>

#include "Tube.h"

namespace COM {

class TaskPackage final : public QThread {
    Q_OBJECT
public:
    enum class State : int {
        Invalid = 0,   // 无效
        ParsingError,  // 解析失败
        Running,       // 运行中
        Pause,         // 暂停
        Stop,          // 停止（被迫停止）
        ForceStop,     // 强制停止
        Accomplish,    // 完成
    };
    Q_ENUM(State)

    struct Info {
        int expValue = -1;
    };

    using smart_entity = std::shared_ptr<TaskPackage>;
    static smart_entity Create_entity(const QJsonObject& json) {
        return smart_entity(new smart_entity::element_type(json));
    }

private:
    /// 原始json，请不要修改
    QJsonObject m_ojson;
    QJsonArray  m_stepsList;
    std::mutex  m_jsonMtx;

private:  /// 注意成员会阻碍copy&move
    std::atomic<State> m_state{State::Invalid};
    Tube::entity_tube  m_tube;
    int                m_taskIndex = -1;

public:
    TaskPackage();
    /// 请在外部保证这是一个合法的json
    TaskPackage(QJsonObject json);
    ~TaskPackage();

public:
    bool operator==(const TaskPackage& rhs) const {
        return qHash(this->m_ojson) == qHash(rhs.m_ojson);
    }
    bool Is_equal(const QJsonObject& json) {
        return qHash(this->m_ojson) == qHash(json);
    }

public:
    /**
     * @brief Reset
     * 重置
     */
    void Reset();

    /**
     * @brief Modify_processStepByIndex
     * 修改 KJ::processSteps 的第 idx 个节点
     * @param idx   对于已设置的 json 的 KJ::processSteps 的长度范围内 [0, n) 越界无效
     * @param json  修改的新值
     */
    void Modify_processStepByIndex(int idx, QJsonObject json);

public:
    Info  Get_Info() const;
    State Get_state() const {
        return m_state.load();
    }

public:
    State Start();
    State Pause();
    State Stop();

private:
    void stop_thread();

protected:
    virtual void run() override;

signals:
    /**
     * @brief signal_state
     * @param state
     * 状态信号
     */
    void signal_state(int state);

    /**
     * @brief signal_valueReceived
     * 最后实验获取的光值
     * @param val
     */
    void signal_valueReceived(int val);

    /**
     * @brief signal_pipetteNeedleSuction
     * 在流程中，移液针所吸取的量（微升）
     * @param idxBinGroup 组号 （范围 [0, 12)）
     * @param idxBinBack  孔位 （范围 [0, 7) 其中0，1，2，3 为试剂孔位 4，5，6
     * 为样本孔位，其他数值为无效值）
     * @param val         吸取量（微升）
     */
    void signal_pipetteNeedleSuction(int idxBinGroup, int idxBinBack, int val);

    /**
     * @brief signal_waring
     * @param msg
     * 故障上报
     * 比如移液针在哪个孔位发生了异常
     * 如吸液异常等等
     */
    void signal_waring(QString msg);
};
}  // namespace COM

#endif  // TASKPACKAGE_H_1718677506

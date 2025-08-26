#ifndef EPIPE_H_1713929741
#define EPIPE_H_1713929741

#include <QDateTime>
#include <QObject>
#include <QTimer>
#include <QVector>
#include <atomic>
#include <cstring>
#include <memory>

namespace COM {
class Tube final
    : public QObject
    , public std::enable_shared_from_this<Tube> {
    Q_OBJECT
public:
    using entity_tube = std::shared_ptr<Tube>;

    static entity_tube Create_new_SmartPipe() {
        return entity_tube(new entity_tube::element_type{});
    }

public:
    enum State {
        State_Invalid,               // 无效
        State_CanBeTransfer,         // 可被转移
        State_Sleeping,              // 休眠中
        State_InMagneticSeparation,  // 在磁分离自旋中
        State_InIncubation,          // 在温裕盘
        State_End,                   // 结束
        State_Missing,               // 丢失
    };
    Q_ENUM(State);

    struct DelayTimePackage {
        static constexpr int timeInterval = 1000;
        int                  timeId       = -1;

        std::function<void()> task;                // 任务
        int                   targetTime = 0;      // 目标时间
        int                   cntTime    = 0;      // 计时时间
        bool                  isAdjusted = false;  // 是否经过了调整

        void Reset() {
            *this = DelayTimePackage{};
        }
    };

    struct ParamPkg {
        int magneticSeparationIndex = -1;
        int incubateX               = -1;
        int incubateY               = -1;
        int sleepTime               = 0;
    };

public:
    ParamPkg paramPkg;

private:
    QVector<QDateTime> m_timeMsgList;
    QVector<QString>   m_strMsgList;

private:
    std::atomic<State> m_state{State_CanBeTransfer};
    DelayTimePackage   m_delayPackage;
    int                m_expValue = -1;

private:
    Tube();

public:
    ~Tube();

public:
    QString to_string() const;

public:
    State Get_state() {
        return m_state.load();
    }

    void Set_state(State state) {
        m_state = state;
    }

    /**
     * 管自己内部信息，本身是否能够
     * 可以继续往下走
     */
    bool Can_Next() {
        return m_state == State_CanBeTransfer;
    }

public:  // 信息记录，只增不删
    void Append_strMsg(QString msg) {
        m_strMsgList.push_back(std::move(msg));
    }

    void Append_timeMsg() {
        m_timeMsgList.push_back(QDateTime::currentDateTime());
    }

public:
    void Set_expValue(int value) {
        m_expValue = value;
        emit signal_valueReceived(m_expValue);
    }

    int Get_expValue() {
        return m_expValue;
    }

public:
    void Set_delayTimeTask(int targetTime, std::function<void()> task);

protected:
    void timerEvent(QTimerEvent* event) override;

signals:
    void signal_valueReceived(int);
    /// 预留，目前未实现
    void signal_pipetteNeedleSuction(int idxBinGroup, int idxBinBack, int val);
};
}  // namespace COM
#endif  // EPIPE_H_1713929741

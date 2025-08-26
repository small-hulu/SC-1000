#ifndef DISPATCHCONTROLLER_H_1718678278
#define DISPATCHCONTROLLER_H_1718678278

#include <QJsonObject>
#include <QList>

#include "../entity/TaskPackage.h"
#include "../interface/ISingleton.hpp"


//该类用于管理所有的TaskPackage

namespace COM {
class DispatchController final
    : public QObject
    , public ISingleton<DispatchController> {
    Q_OBJECT
public:
    enum class State {
        Invaild,     // 无效
        Leisure,     // 空闲
        Running,     // 运行中
        Pause,       // 暂停
        Stop,        // 停止
        ForceStop,   // 强制停止
        Accomplish,  // 完成
    };
    Q_ENUM(State);

private:
    State m_state{State::Leisure};

private:
    using TaskPackageSet = std::unordered_map<int, TaskPackage::smart_entity>;
    std::unordered_map<int, TaskPackage::smart_entity> m_taskSet;

public:
    DispatchController();

    QString to_string() const;

public:
    void  Reset();
    State Get_state() {
        return m_state;
    }
    void Set_state(int flag) {
        m_state = (State)flag;
    }
    TaskPackageSet &Get_taskList() {
        return m_taskSet;
    }

public:
    /// 用户添加一个实验任务
    TaskPackage::smart_entity Append_taskPackage(int key, const QJsonObject &ojson);
    TaskPackage::smart_entity Append_taskPackage(const QJsonObject &ojson);
    /// 用户移除一个实验任务
    bool Remove_taskPackage(int key);

public:  // 对每个实验个体做转发，并统一管理整理状态
    /// 停止所有实验
    State Stop_dispatch();
    /// 暂停
    State Pause_dispatch();
    /// 对每一个package执行Run
    /// 分为从0开始和暂停再启动
    State Start_dispatch();

signals:
    void signal_Accomplish();
    void signal_ForceStop();
};
}  // namespace COM
#endif  // DISPATCHCONTROLLER_H_1718678278

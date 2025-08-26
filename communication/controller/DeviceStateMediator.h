/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       DeviceStateMediator.h
 * @date       2024-06-18
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 设备状态中介者
 * ===============================================
 * 能进入这个中介者，说明设备的mutex已经获取了
 * 并且操作中介者是在核心动作的前后，因此可以直接操作部件
 * ===============================================
 * 这里不要使用RAII，请直接手动调用
 * 因为当获取状态后，可能中间的操作是失败的
 * 不能强制RAII去执行首尾操作
 * ===============================================
 * 目前只考虑，Minor模式以from-to的模式
 *****************************************************************************/
#ifndef DEVICESTATEMEDIATOR_H_1718680157
#define DEVICESTATEMEDIATOR_H_1718680157

#include <QJsonObject>
#include <QObject>

#include "../device/DeviceManager.h"
#include "../entity/Tube.h"

namespace COM {
class DeviceStateMediator final : public QObject {
    Q_OBJECT
public:
    enum State {
        Invalid,     // 无效
        Success,     // 成功
        Again,       // 重试
        ForceStop,   // 强制停止
        StateError,  // 状态错误
        ParseError,  // 解析错误
    };
    Q_ENUM(State)

private:
    QJsonObject                m_json;
    Tube::entity_tube          m_tube;
    DeviceManager::atomic_flag m_fromType{};
    DeviceManager::atomic_flag m_toType{};

public:
    DeviceStateMediator(const QJsonObject& json, Tube::entity_tube tube, QObject* parent = nullptr);

public:
    QJsonObject& Get_json() {
        return m_json;
    }
    Tube::entity_tube Get_tube() {
        return m_tube;
    }

public:
    State Check_stateCU();
    State Modify_stateCU();

private:
    State check_from_stateCU();
    State modify_from_stateCU();
    State check_to_stateCU();
    State modify_to_stateCU();
};
}  // namespace COM

#endif  // DEVICESTATEMEDIATOR_H_1718680157

#ifndef MODBUS_H_1688113713
#define MODBUS_H_1688113713

#include <QDebug>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusDevice>
#include <QModbusRequest>
#include <QObject>
#include <QSerialPortInfo>
#include <QString>
#include <QThread>
#include <mutex>
#include <queue>

#include "../interface/ICOMThread.h"

namespace COM {
/// [struct]
/**
 * 通信reply的一个包装，
 * 因为`QModbusReply` is a `QObject`所以不适合传递
 * 目前不考虑 rawData
 */
struct ModbusReplyWrapper {
    using Error = QModbusDevice::Error;

    Error           error = Error::UnknownError;
    QModbusDataUnit data;

    inline bool is_valid() const {
        return error == Error::NoError;
    }
};
/// [struct]

/// [func]
extern QModbusClient* Create_modbus_rtu();
/// [func]

/// [class]
class ModbusImpl final : public ICOMThread {
    Q_OBJECT
public:
    using reply_smart = QScopedPointer<QModbusReply, QScopedPointerDeleteLater>;
    using task_type   = std::function<void()>;

    struct Info {
        QString port;
        int     vid = -1;
        int     pid = -1;
    };

public:
    using Self = ModbusImpl;
    static Self& instance() {
        static Self obj{};
        return obj;
    }

protected:
    QModbusClient* m_modbus = nullptr;
    Info           m_info;

protected:
    std::queue<task_type> m_taskQueue;
    size_t                m_timeTaskInterval = 10;

public:
    explicit ModbusImpl(QObject* parent = nullptr);

public:
    void Set_info(Info info) {
        m_info = info;
    }

public:
    bool                 open();
    bool                 open(const QString& port);
    void                 close();
    bool                 isOpen();
    QModbusDevice::Error error() const;

public:
    ModbusReplyWrapper read(int deviceAddress, const QModbusDataUnit& data);
    ModbusReplyWrapper write(int deviceAddress, const QModbusDataUnit& data);

public:
    template <typename Fun, typename... Args>
    auto Append_Task(Fun&& fun, Args&&... args)
        -> std::shared_ptr<std::future<std::result_of_t<Fun(Args...)>>> {
        using return_type = std::result_of_t<Fun(Args...)>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<Fun>(fun), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(th_taskMutex);
            m_taskQueue.emplace([task]() { (*task)(); });
        }
        th_taskCV.notify_one();
        return std::make_shared<std::future<return_type>>(std::move(res));
    }

protected:
    virtual void run() override;
};
/// [class]

}  // namespace COM

#endif  // MODBUS_H_1688113713

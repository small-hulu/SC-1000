#ifndef LTE_ML302_H
#define LTE_ML302_H

#include <QByteArray>
#include <QCoreApplication>
#include <QDebug>
#include <QQueue>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QTextCodec>
#include <QTime>
#include <QTimer>
#include <iostream>
#include <thread>
#include <tuple>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <QtConcurrent>
#include <QThread>
#include "./lte/controller/HashCalculator.h"
#include "./lte/interface/ICOMThread.h"
using namespace ::std;
/**
 *  此类是对LTE_ML302模块通用功能进行封装。
 *  有获取经纬度，信号强度，与TCP连接，心跳，重连，传输等功能。
 *  LTE_ML302类中维护了一个内部类Result，目的是返回此模块与TCP交互时返回的数据进行一个封装。 
 * */
class LTE_ML302 : public ICOMThread {
    Q_OBJECT
public:
    using task_type = std::function<void()>;
    std::queue<task_type> m_taskQueue;
    size_t                m_timeTaskInterval = 10; // ms

    class Result {
    public:
        Result() = default;
        Result(bool status, QString data) {
            this->status = status;
            this->data   = data;
        }

    private:
        bool    status = false;
        QString data;

    public:
        void setStatus(bool _status) {
            status = _status;
        }
        bool getStatus() {
            return status;
        }
        void setData(QString _data) {
            data = _data;
        }
        QString getData() {
            return data;
        }
    };
public:  // 公有方法
    static LTE_ML302 *get_instance() {
        if (lte_ml302 == nullptr) {
            lte_ml302 = new LTE_ML302();
        }
        return lte_ml302;
    }
    static LTE_ML302 *get_instance(QString equipment_number) {
        if (lte_ml302 == nullptr) {
            lte_ml302 = new LTE_ML302(equipment_number);
        }
        return lte_ml302;
    }
    bool              send_connect();
    void              send_heartbeat();
    void              open_debug();
    void              close_debug();
    LTE_ML302::Result send_text_to_tcp_recursive(const QString &data);
    bool              open_serial_port(QString com);
    bool              open_serial_port(QString vid, QString pid);
    void              close_serial_port();
    bool              is_open_serial();
    QStringList       get_coordinates();
    QString           get_cimi();
    QString           get_csq();
    QString           getIp() const;
    bool              open_tcp_connect(QString ip, QString port);
    bool              close_tcp_connect();
    QByteArray        http_get(QString url);
    QByteArray        http_download(QString url);
    bool              network_card_mode(bool flag);
    LTE_ML302::Result send_text_to_tcp(QString data);
    LTE_ML302::Result parse(QString data);
    // 设置设备编号
    void set_equipment_number(QString equipment_number) {
        this->equipment_number = equipment_number;
    };
    // 设置重连间隔时长，单位分钟，默认五分钟
    void set_reconnect_interval(int num) {
        reconnect_interval = num;
    };
    // 设置TCP连接心跳间隔时长，单位分钟，默认五分钟
    void set_heartbeat_interval(int num) {
        heartbeat_interval = num;
    };

    QString getEquipment_number() const;

    QString getKey();
    void setKey(const QString &value);
    QJsonObject parse_to_json(const QString &value);
private:
    LTE_ML302();
    LTE_ML302(QString equipment_number);

private:  // 私有成员变量
    static LTE_ML302 *lte_ml302;
    bool debug = false;
    QSerialPort *serial = nullptr;
    // 当前给模块发送的指令
    QString    command;
    QByteArray result;
    QString    ip;
    QString    port;
    bool       connected = false;
    QString    equipment_number;
    QString    key;
    // 重连间隔时长：默认五分钟
    int    reconnect_interval = 5;
    QTimer *reconnect_timer = nullptr;
    // 心跳间隔时长：默认五分钟
    int    heartbeat_interval = 5;
    QTimer *heartbeat_timer = nullptr;
    QQueue<QString> data_queue;

private:
    QByteArray send_command_sync_priv(QString command);
    bool       send_command_sync_bool_priv(QString command);
    QByteArray send_command_async_priv(QString command);
    QByteArray send_command_priv(QString command, bool sync);

    void       init();
    void       prepare_connect();
    bool       is_tcp_connected();
    void       tcp_reconnect();

    bool       restart();
    QString    parse_tcp_send(QString data);
    void       delay(int num);
    QString    append_line(QString str);
    void       log(QString str);

    template <typename T>
    /**
     *  对QList进行安全访问
     *  @param list 需要访问的QList
     *  @param index 访问下标
     */
    T safe_access(const QList<T> &list, int index) {
        if (index >= 0 && index < list.size()) {
            return list.at(index);
        } else {
            return NULL;
        }
    };

public:
    // 线程安全封装
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
    void run() override;


private slots:
    void reconnect();
    void handle_ready_read();
signals:
    void receive_from_tcp(QJsonObject json);
};

#endif  // LTE_ML302_H

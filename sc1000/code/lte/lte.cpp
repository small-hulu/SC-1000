#include "lte.h"

LTE_ML302 *LTE_ML302::lte_ml302 = NULL;

/**
 *  构造函数
 */
LTE_ML302::LTE_ML302(): ICOMThread(nullptr)  {
    setObjectName("LTE_ML302");
    init();
    th_isRunning = true;

    this->Start_thread();
}

LTE_ML302::LTE_ML302(QString equipment_number) {
    setObjectName("LTE_ML302");

    init();
    this->equipment_number = equipment_number;
    this->Start_thread();

}


/**
 *  初始化
 */
void LTE_ML302::init() {

    // 设置字符集
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    reconnect_timer = new QTimer();
    heartbeat_timer = new QTimer();

    if (serial) {
        if (serial->isOpen()) serial->close();
        serial->deleteLater();
    }

    serial = new QSerialPort();
    serial->setParity(QSerialPort::NoParity);
    serial->setDataBits(QSerialPort::Data8);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial, &QSerialPort::readyRead, this, &LTE_ML302::handle_ready_read, Qt::DirectConnection);
}



QString LTE_ML302::getKey()
{
    auto task = [this]() -> QString {
        QString response = QString::fromUtf8(send_command_sync_priv("AT+LBSKEY?"));
        if (!response.contains("OK"))
            return {};
        auto parts = response.split("\r\n", QString::SkipEmptyParts);
        for (const auto &line : parts) {
            if (line.startsWith("+LBSKEY:")) {
                return line.mid(QString("+LBSKEY:").length()).trimmed();
            }
        }
        return {};
    };
    auto fut = Append_Task(task);
    return fut->get();
}


/// KEYvalue : 0bbfbf7b1153997df1a86afdb3656c20
void LTE_ML302::setKey(const QString &value)
{
    key = value;
}

QJsonObject LTE_ML302::parse_to_json(const QString &value) {
    QJsonObject obj;
    QStringList parts = value.split(",", Qt::KeepEmptyParts); // 保留空字段

    if (parts.size() < 6) {
        obj["error"] = "invalid format";
        return obj;
    }

    QString operate = parts[2].trimmed();
    QString actionType    = parts[3].trimmed();
    QString dataValue  = parts[4].trimmed();
    QString time       = parts[5].trimmed();

    obj["message"]    = " ";
    obj["operate"]    = operate;
    obj["actionType"] = actionType;
    obj["value"]      = dataValue.isEmpty() ? " " : dataValue;
    obj["time"]       = time;

    return obj;
}

QString LTE_ML302::getEquipment_number() const {
    return equipment_number;
}

/**
 *  打开串口
 *  @param com 串口号
 *  @return 结果
 */
bool LTE_ML302::open_serial_port(QString com) {
    auto task = [this, com]() -> bool {
        if (!serial) {
            serial = new QSerialPort();

            serial->setParity(QSerialPort::NoParity);
            serial->setDataBits(QSerialPort::Data8);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);

            // readyRead 信号只在串口线程连接
            connect(serial, &QSerialPort::readyRead, this, &LTE_ML302::handle_ready_read, Qt::DirectConnection);
        }
        if (serial->isOpen()) serial->close();
        serial->setBaudRate(115200);
        serial->setPortName(com);
        if (!serial->open(QIODevice::ReadWrite)) {
            qWarning() << "open failed:" << serial->errorString();
            return false;
        }
        send_command_sync_priv("ATE0");
        log(QString("串口已打开：%1").arg(com));
        return true;
    };
    auto fut = Append_Task(task);
    return fut->get();
}

bool LTE_ML302::open_serial_port(QString vid, QString pid) {
    auto task = [this, vid, pid]() -> bool {
        if (!serial) {
            serial = new QSerialPort();
            serial->setParity(QSerialPort::NoParity);
            serial->setDataBits(QSerialPort::Data8);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            connect(serial, &QSerialPort::readyRead, this, &LTE_ML302::handle_ready_read, Qt::DirectConnection);
        }
        if (serial->isOpen()) serial->close();

        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            if (info.isValid() && info.hasVendorIdentifier() && info.hasProductIdentifier()) {
                if (QString::number(info.vendorIdentifier()) == vid &&
                    QString::number(info.productIdentifier()) == pid) {
                    serial->setPort(info);
                    serial->setBaudRate(115200);
                    if (serial->open(QIODevice::ReadWrite)) {
                        send_command_sync_priv("ATE0");
                        log(QString("串口已打开：%1 (VID=%2, PID=%3)")
                                .arg(info.portName()).arg(vid, pid));
                        return true;
                    } else {
                        qWarning() << "open failed:" << serial->errorString();
                        return false;
                    }
                }
            }
        }
        return false;
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  关闭串口
 */
void LTE_ML302::close_serial_port() {
    auto task = [this]() {
        if (reconnect_timer && reconnect_timer->isActive()) reconnect_timer->stop();
        if (heartbeat_timer && heartbeat_timer->isActive()) heartbeat_timer->stop();

        if (is_tcp_connected() && close_tcp_connect()) {
            log("TCP连接已关闭");
        }
        if (serial) {
            if (serial->isOpen()) serial->close();
            log("串口已关闭");
        }
    };
    auto fut = Append_Task(task);
    fut->get();
}


/**
 *  是否打开串口
 *  @return 结果
 */
bool LTE_ML302::is_open_serial() {
    auto task = [this]() -> bool {
        return serial && serial->isOpen();
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  打开TCP连接
 *  @param ip 服务器IP地址
 *  @param port 端口号
 *  @return 结果
 */
bool LTE_ML302::open_tcp_connect(QString _ip, QString _port) {
    auto task = [this, _ip, _port]() -> bool {
        if (equipment_number.isEmpty()) throw QString("请先设置设备编号");

        ip   = _ip;
        port = _port;
        prepare_connect();
        if (is_tcp_connected()) return true;

        QString command = QString("AT+MIPOPEN=1,\"TCP\",\"%1\",%2").arg(ip, port);
        QString result  = QString::fromUtf8(send_command_sync_priv(command));
        if (result.contains("OK")) {
            log(QString("连接到TCP服务器：%1:%2").arg(ip, port));
            connected = true;

            return true;
        }
        return false;
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  TCP连接前准备：发送一系列TCP连接前的AT指令，停止重连，清空数据队列
 */
void LTE_ML302::prepare_connect() {
    send_command_sync_priv("AT");
    send_command_sync_priv("ATE0");
    send_command_sync_priv("AT+CGDCONT=1,\"IP\",\"CMNET\"");
    send_command_sync_priv("AT+CGACT=1,1");
    send_command_sync_priv("AT+CSTT=\"CMNET\"");
    send_command_sync_priv("AT+CGATT=1");
    send_command_sync_priv("AT+LBSKEY=\"0bbfbf7b1153997df1a86afdb3656c20\"");
    if (reconnect_timer && reconnect_timer->isActive()) reconnect_timer->stop();
    data_queue.clear();
}
/**
 *  关闭TCP连接
 */
bool LTE_ML302::close_tcp_connect() {
    auto task = [this]() -> bool {
        if (!is_tcp_connected()) throw QString("未连接到TCP，无需关闭连接");
        QString result = QString::fromUtf8(send_command_sync_priv("AT+MIPCLOSE=1"));
        if (result.contains("OK")) {
            connected = false;
            return true;
        }
        return false;
    };
    auto fut = Append_Task(task);
    return fut->get();
}
/**
 *  发送TCP连接包
 */
bool LTE_ML302::send_connect() {
    LTE_ML302::Result result = send_text_to_tcp(equipment_number + "-ClientConnect");
    return result.getStatus() && result.getData().contains("ConnectOK");
}

/**
 *  发送TCP心跳包
 */
void LTE_ML302::send_heartbeat() {
    if (!heartbeat_timer) return;
    if (heartbeat_timer->isActive()) return;
    heartbeat_timer->setInterval(heartbeat_interval * 60 * 1000);
    connect(heartbeat_timer, &QTimer::timeout, this, [this]() {
        qDebug()<<"heartbeat_timer"<<endl;
        LTE_ML302::Result result = send_text_to_tcp(this->equipment_number + "-ClientOnline");
        connected = result.getStatus() && result.getData().contains("OnlineOK");
    }, Qt::UniqueConnection);

    // 立即触发一次
    send_text_to_tcp(equipment_number + "-ClientOnline");
    heartbeat_timer->start();
}

/**
 *  是否连接了TCP服务器
 */
bool LTE_ML302::is_tcp_connected() {
    return connected;
}


/**
 *  获取经纬度
 *  @return 经纬度信息，获取失败则经纬度均返回0
 */
QStringList LTE_ML302::get_coordinates() {
    auto task = [this]() -> QStringList {
        if (send_command_sync_bool_priv("AT+CGACT=1")) {
            QString result = QString::fromUtf8(send_command_sync_priv("AT+QCELLLOC"));
            if (!result.contains("OK")) return {"0", "0"};
            return safe_access(safe_access(result.split("\r\n", QString::SkipEmptyParts), 0)
                                   .split("+QCELLLOC: "), 1)
                .split(",");
        }
        return {"0", "0"};
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  获取cimi
 *  @return 结果
 */
QString LTE_ML302::get_cimi() {
    auto task = [this]() -> QString {
        QString result = QString::fromUtf8(send_command_sync_priv("AT+CIMI"));
        if (!result.contains("OK")) return {};
        return safe_access(result.split("\r\n", QString::SkipEmptyParts), 0);
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  获取csq(信号强度)
 *  @return 结果
 */
QString LTE_ML302::get_csq() {
    auto task = [this]() -> QString {
        QString result = QString::fromUtf8(send_command_sync_priv("AT+CSQ"));
        if (!result.contains("OK")) return {};
        return safe_access(
            safe_access(safe_access(result.split("\r\n", QString::SkipEmptyParts), 0).split("+CSQ: "),
                        1)
                .split(","),
            0);
    };
    auto fut = Append_Task(task);
    return fut->get();
}
/**
 *  向TCP服务器发送文本数据
 *  @param data 文本数据
 *  @return TCP返回结果
 */
LTE_ML302::Result LTE_ML302::send_text_to_tcp(QString data) {

    auto task = [this, data]() -> Result {
        if (!is_tcp_connected()) {
            if (ip.isEmpty()) throw QString("未连接TCP服务器！");
            data_queue.enqueue(data);
            tcp_reconnect();
            return Result(false, {});
        }
        QString d = data;
        if (d.contains("\r\n")) d.remove("\r\n");
        send_command_sync_priv(QString("AT+MIPSEND=1,%1").arg(d.toUtf8().size() + 1));
        return parse(QString::fromUtf8(send_command_sync_priv(d)));
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  递归发送，直到成功
 */
LTE_ML302::Result LTE_ML302::send_text_to_tcp_recursive(const QString &data) {
    auto task = [this, data]() -> Result {
        static int retryCount = 0;
        Result res = send_text_to_tcp(data);
        if (!res.getStatus()) {
            retryCount++;
            if (retryCount >= 10) {
                return res;
            }
            // 递归重试（注意：仍在串口线程中执行）
            return send_text_to_tcp_recursive(data);
        }
        retryCount = 0;
        return res;
    };
    auto fut = Append_Task(task);
    return fut->get();
}

QByteArray LTE_ML302::send_command_sync_priv(QString cmd) {
    return send_command_priv(std::move(cmd), true);
}

bool LTE_ML302::send_command_sync_bool_priv(QString cmd) {
    return !send_command_priv(std::move(cmd), true).contains("CME");
}

QByteArray LTE_ML302::send_command_async_priv(QString cmd) {
    return send_command_priv(std::move(cmd), false);
}

QByteArray LTE_ML302::send_command_priv(QString cmd, bool sync) {
    log("LTE_ML302 send: " + cmd);

    command = cmd;
    cmd     = append_line(cmd);

    if (!serial) return {};
    serial->clear();
    serial->write(cmd.toUtf8());
    serial->waitForBytesWritten(1000);

    if (sync) {
        if (!serial->waitForReadyRead(5000)) {
            return {};
        }
        delay(50);
        QByteArray result = serial->readAll();
        log("receive_sync: " + QString::fromUtf8(result));
        if (result.contains("PMD:Control")) {
            QStringList lines = QString(result).split("\r\n", QString::SkipEmptyParts);
            QString     result_str;
            for (const QString& str : lines) {
                if (str.contains("PMD:Control")) {
                    QString hash_str = parse_tcp_send(result) + "\r\n";
                    QJsonObject json = parse_to_json(parse_tcp_send(result));
                    QString hash = HashCalculator::md5(hash_str);
                    json["hash"] = hash;

                    qDebug().noquote() << QJsonDocument(json).toJson(QJsonDocument::Indented);
                    emit receive_from_tcp(json);
                } else {
                    result_str.append(str);
                }
            }
            command.clear();
            return result_str.toUtf8();
        }
        command.clear();
        return result;
    }
    return {};
}



/**
 *  TCP重连
 */
void LTE_ML302::tcp_reconnect() {
    if (!reconnect_timer) return;
    if (reconnect_timer->isActive()) return;
    reconnect_timer->setInterval(reconnect_interval * 60 * 1000);
    connect(reconnect_timer, &QTimer::timeout, this, &LTE_ML302::reconnect, Qt::UniqueConnection);
    reconnect_timer->start();
}

/**
 *  重连槽函数
 */
void LTE_ML302::reconnect() {
    QString command = QString("AT+MIPOPEN=1,\"TCP\",\"%1\",%2").arg(ip, port);
    QString result  = QString::fromUtf8(send_command_sync_priv(command));
    if (result.contains("OK")) {
        reconnect_timer->stop();
        connected = true;
        send_connect();
        send_heartbeat();
        while (!data_queue.isEmpty()) {
            QString data = data_queue.dequeue();
            delay(1000);
            send_text_to_tcp(data);
        }
    }
}

/**
 *  处理TCP主动发送的数据
 */

void LTE_ML302::handle_ready_read() {
    if (command.isEmpty()) {
        delay(50);
        QString r = serial->readAll();
        if (r.contains("PMD:Control")) {
            log("receive from tcp: " + r);
            QString     hash_str = parse_tcp_send(r) + "\r\n";
            QJsonObject json     = parse_to_json(parse_tcp_send(r));
            QString     hash     = HashCalculator::md5(hash_str);
            json["hash"] = hash;

            qDebug().noquote() << QJsonDocument(json).toJson(QJsonDocument::Indented);
            emit receive_from_tcp(json);
        }

    }
}
/**
 *  发送HTTP Get请求
 *  @param url 请求地址
 *  @return http返回结果
 */
QByteArray LTE_ML302::http_get(QString url) {
    auto task = [this, url]() -> QByteArray {
        send_command_sync_priv("ATE0");
        send_command_sync_priv("AT+CGACT=1,1");
        send_command_sync_priv("AT+MHTTPINIT");
        send_command_sync_priv(QString("AT+MHTTPSET=\"%1\"").arg(url));

        send_command_async_priv("AT+MHTTPGET");
        if (serial->waitForReadyRead(5000)) {
            QString status = serial->readAll();
            if (status.contains("OK") && serial->waitForReadyRead(5000)) {
                QString     result = serial->readAll();
                QStringList list   = result.split("\r\n")[0].split("+MHTTPGET:")[1].split(",");
                if (list[0] == "200") {
                    int size = list[1].toInt();
                    send_command_async_priv(QString("AT+MHTTPREAD=0,%1").arg(size));
                    if (serial->waitForReadyRead(5000)) {
                        serial->readLine();
                        status = serial->readLine().trimmed();
                        if (status == "OK") {
                            serial->readLine();
                            QByteArray data = serial->readAll().trimmed();
                            command.clear();
                            return data;
                        }
                    }
                }
            }
        }
        command.clear();
        return {};
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  发送HTTP 下载文件请求
 *  @param url 请求地址
 *  @return 文件字节数组
 */
QByteArray LTE_ML302::http_download(QString url) {
    auto task = [this, url]() -> QByteArray {
        send_command_sync_priv("ATE0");
        send_command_sync_priv("AT+CGACT=1,1");
        send_command_sync_priv("AT+MHTTPINIT");
        send_command_sync_priv(QString("AT+MHTTPSET=\"%1\"").arg(url));

        send_command_async_priv("AT+MHTTPDL");
        if (serial->waitForReadyRead(5000)) {
            serial->readLine();
            QString line2 = serial->readLine();
            if (line2.contains("OK") && serial->waitForReadyRead(5000)) {
                delay(100);
                QString     line3       = serial->readLine();
                QStringList result_head = line3.split("\r\n")[0].split("+MHTTPDL:")[1].split(",");
                if (result_head[0] == "200") {
                    int size = result_head[1].toInt();
                    QByteArray byteArray;
                    while (byteArray.size() < size) {
                        QByteArray data = serial->readAll();
                        if (data.endsWith("\r\n")) data.chop(2);
                        log(QString("本次下载大小：%1").arg(data.size()));
                        byteArray.append(data);
                        if (byteArray.size() == size) break;
                        if (!serial->waitForReadyRead(5000)) break;
                    }
                    command.clear();
                    return byteArray;
                }
            }
        }
        command.clear();
        return {};
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  是否切换为网卡模式
 *  @param flag 打开或关闭标志
 *  @return 结果
 */
bool LTE_ML302::network_card_mode(bool flag) {
    auto task = [this, flag]() -> bool {
        if (flag) {
            if (!send_command_sync_bool_priv("AT+SYSNV=1,\"usbmode\",3")) return false;
        } else {
            if (!send_command_sync_bool_priv("AT+SYSNV=1,\"usbmode\",2")) return false;
        }
        if (!restart()) return false;
        return send_command_sync_bool_priv("AT+CGDCONT=1,\"IP\",\"CMNET\"") &&
               send_command_sync_bool_priv("AT+CGACT=1,1");
    };
    auto fut = Append_Task(task);
    return fut->get();
}

/**
 *  重启模组
 *  @return 是否重启成功
 */
bool LTE_ML302::restart() {
    send_command_async_priv("AT+TRB");
    delay(1000);
    QString r = serial ? serial->readAll() : QString();
    if (!r.contains("OK")) {
        return false;
    }
    int maxCount = 15;
    for (int i = 0; i < maxCount; i++) {
        delay(500);
        send_command_async_priv("AT");
        delay(500);
        QString result = serial->readAll();
        if (result.contains("OK")) {
            break;
        }
    }
    send_command_sync_priv("ATE0");
    return true;
}


/**
 *  在字符串末尾追加换行符
 *  @param str 待处理字符串
 *  @return 处理后的字符串，末尾追加换行符
 */
QString LTE_ML302::append_line(QString str) {
    QString result = str;
    // 若存在，则去掉
    if (result.contains("\r\n")) {
        result.remove("\r\n");
    }
    return result + "\r\n";
}

/**
 *  解析TCP回复的数据
 *  @param data 待解析的数据
 *  @return 解析结果
 */
LTE_ML302::Result LTE_ML302::parse(QString data) {
    bool status = false;
    QString recv;
    if (data.contains("SEND OK")) {
        status = true;
        if (data.contains("recv")) recv = parse_tcp_send(data);
        else recv = "";
    }
    return Result(status, recv);
}


/**
 *  解析TCP主动发送的数据
 *  @param data 待解析的数据
 *  @return 解析结果
 */
QString LTE_ML302::parse_tcp_send(QString data) {
    return data.split("recv")[1].split("\r\n", QString::SkipEmptyParts)[1];
}


/**
 *  打开debug
 */
void LTE_ML302::open_debug() {
    debug = true;
}

/**
 *  关闭debug
 */
void LTE_ML302::close_debug() {
    debug = false;
}

/**
 *  日志打印
 *  @param str 待处理字符串
 */
void LTE_ML302::log(QString str) {
    if (debug) {
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        qDebug() << qPrintable(timestamp) << "[debug]" << str;
    }
}

/**
 *  延迟函数
 *  @param num 延迟毫秒数
 */
void LTE_ML302::delay(int num) {
    // 创建一个新的事件循环对象
    QEventLoop loop;
    // 创建一个单次定时器，延迟时间为num毫秒，槽函数为事件循环的退出函数
    QTimer::singleShot(num, &loop, SLOT(quit()));
    // 开始执行事件循环，程序会在这里暂停执行，直到定时时间到达或者事件循环被显式退出
    loop.exec();
}

/* ---------------- 串口线程主循环 ---------------- */
void LTE_ML302::run() {
    QEventLoop eventLoop;

    while (th_isRunning) {
        task_type task{};
        {
            std::unique_lock<std::mutex> lock(th_taskMutex);

            th_taskCV.wait_for(lock, std::chrono::milliseconds(100),
                             [this]() { return !m_taskQueue.empty() || !th_isRunning; });

            if (!th_isRunning) break;

            if (m_taskQueue.empty()) {
                lock.unlock();
                // 处理pending的事件
                eventLoop.processEvents(QEventLoop::AllEvents, 50);
                continue;
            }

            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
        }

        if (task) {
            eventLoop.processEvents(QEventLoop::AllEvents, 10);

            QThread::msleep(m_timeTaskInterval);
            task();

            eventLoop.processEvents(QEventLoop::AllEvents, 10);
        }
    }

    // 清理
    if (reconnect_timer) {
        reconnect_timer->stop();
        reconnect_timer->deleteLater();
        reconnect_timer = nullptr;
    }
    if (heartbeat_timer) {
        heartbeat_timer->stop();
        heartbeat_timer->deleteLater();
        heartbeat_timer = nullptr;
    }

    if (serial) {
        if (serial->isOpen()) serial->close();
        QMetaObject::invokeMethod(serial, "deleteLater", Qt::QueuedConnection);
        serial = nullptr;
    }

    qDebug() << "LTE_ML302线程退出";
}


QString LTE_ML302::getIp() const
{
    return ip;
}

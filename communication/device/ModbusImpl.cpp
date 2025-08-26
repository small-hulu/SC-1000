/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       MODBUS_ModbusBase.cpp
 * @date       2024-01-17
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief
 * 从V0.29版本开始，modbus改成自循环的方式
 *
 *
 *****************************************************************************/
#include "ModbusImpl.h"

#include <QDebug>
#include <QEventLoop>
#include <QModbusClient>
#include <QModbusReply>
#include <QModbusRtuSerialMaster>
#include <QScopedPointer>
#include <QSerialPort>
#include <QThread>
#include <QTimer>
#include <cstdlib>
#include <iostream>

#include "../utility/IniConfig.h"

/** Define Begin *************************************************************/

#define ON_DEBUG_MODBUS 0

#define DEBUG_MODBUS()      \
    if (!ON_DEBUG_MODBUS) { \
    } else                  \
        qDebug()

/** Define End ***************************************************************/

namespace COM {
QModbusClient *Create_modbus_rtu() {
    auto &ini    = IniConfig::instance();
    auto  modbus = new QModbusRtuSerialMaster{};
    modbus->setTimeout(ini[KI::modbus_timeout].toInt());
    modbus->setNumberOfRetries(ini[KI::modbus_numberOfRetries].toInt());
    modbus->setConnectionParameter(QModbusDevice::SerialParityParameter, QSerialPort::NoParity);
    modbus->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, QSerialPort::Data8);
    modbus->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, QSerialPort::OneStop);
    modbus->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, QSerialPort::Baud115200);
    return modbus;
}

ModbusImpl::ModbusImpl(QObject *parent) : ICOMThread(parent) {
    setObjectName(__func__);
    auto &ini          = IniConfig::instance();
    m_timeTaskInterval = ini[KI::time_taskInterval].toUInt();

    /// 优先级提到最高
    this->Start_thread();
    this->setPriority(QThread::Priority::HighestPriority);
}

/**
 * open & close
 * 要和modbus保持在一个线程
 */
bool ModbusImpl::open() {
    auto task = [this]() -> bool {
        if (nullptr == m_modbus) {
            qWarning() << QString("Modbus is nullptr");
            return false;
        }
        m_modbus->setConnectionParameter(QModbusDevice::SerialPortNameParameter, m_info.port);

        return m_modbus->connectDevice();
    };

    auto fut = Append_Task(task);
    auto ret = fut->get();

    qInfo() << "Modebus exec open" << m_modbus->state();
    return ret;
}

bool ModbusImpl::open(const QString &port) {
    m_info.port = port;
    return open();
}

/**
 * open & close
 * 要和modbus保持在一个线程
 */
void ModbusImpl::close() {
    auto task = [this]() {
        if (nullptr == m_modbus) {
            return;
        }
        if (this->isOpen()) {
            m_modbus->disconnectDevice();
        }
    };

    auto fut = Append_Task(task);
    fut->get();
    qInfo() << "Modebus exec close" << m_modbus->state();
}

/**
 * 单纯读一下状态
 */
bool ModbusImpl::isOpen() {
    if (nullptr == m_modbus) {
        return false;
    }
    return m_modbus->state() == QModbusDevice::ConnectedState;
}

/**
 * 单纯读一下状态
 */
QModbusDevice::Error ModbusImpl::error() const {
    if (nullptr == m_modbus) {
        return QModbusDevice::Error::UnknownError;
    }
    return m_modbus->error();
}

/**
 * send·Read·Request
 */
ModbusReplyWrapper ModbusImpl::read(int deviceAddress, const QModbusDataUnit &data) {
    auto task = [this, deviceAddress, data]() -> ModbusReplyWrapper {
        DEBUG_MODBUS() << "READ[1] enter" << deviceAddress;
        QEventLoop  loop;
        reply_smart reply{m_modbus->sendReadRequest(data, deviceAddress)};
        DEBUG_MODBUS() << "READ[2] get reply" << deviceAddress;

        if (reply == nullptr) {
            qWarning() << "READ[-1] reply == nullptr" << deviceAddress;
            return {};
        } else {
            connect(reply.get(), &QModbusReply::finished, &loop, &QEventLoop::quit);
        }

        if (reply->isFinished() == false) {
            int ret = loop.exec();
            if (ret != 0) {
                qWarning() << "loop ret = " << ret;
            }
        } else {
            qCritical() << ">>>>>reply advanced Finished!!!";
        }
        DEBUG_MODBUS() << "READ[3] reply finished" << reply->error() << deviceAddress;
        if (reply->error() != QModbusDevice::Error::NoError) {
            qCritical() << "WRITE: " << reply->error() << deviceAddress;
        }

        ModbusReplyWrapper replyWrapper;
        replyWrapper.error = reply->error();
        replyWrapper.data  = reply->result();
        return replyWrapper;
    };

    auto fut = Append_Task(task);
    return fut->get();
}

/**
 * send·Write·Request
 */
ModbusReplyWrapper ModbusImpl::write(int deviceAddress, const QModbusDataUnit &data) {
    auto task = [this, deviceAddress, data]() -> ModbusReplyWrapper {
        DEBUG_MODBUS() << "WRITE[1] enter" << deviceAddress;
        QEventLoop  loop;
        reply_smart reply{m_modbus->sendWriteRequest(data, deviceAddress)};
        DEBUG_MODBUS() << "WRITE[2] get reply" << deviceAddress;

        if (reply == nullptr) {
            qWarning() << "READ[-1] reply == nullptr" << deviceAddress;
            return {};
        } else {
            connect(reply.get(), &QModbusReply::finished, &loop, &QEventLoop::quit);
        }

        if (reply->isFinished() == false) {
            int ret = loop.exec();
            if (ret != 0) {
                qWarning() << "loop ret = " << ret;
            }
        } else {
            qCritical() << ">>>>>reply advanced Finished!!!" << deviceAddress;
        }
        DEBUG_MODBUS() << "WRITE[3] reply finished" << reply->error() << deviceAddress;
        if (reply->error() != QModbusDevice::Error::NoError) {
            qCritical() << "WRITE: " << reply->error() << deviceAddress;
        }

        ModbusReplyWrapper replyWrapper;
        replyWrapper.error = reply->error();
        replyWrapper.data  = reply->result();
        return replyWrapper;
    };

    auto fut = Append_Task(task);
    return fut->get();
}

void ModbusImpl::run() {
    /// [modbus-init]
    if (m_modbus) {
        if (m_modbus->state() == QModbusDevice::ConnectedState) {
            m_modbus->disconnectDevice();
        }
        m_modbus->deleteLater();
    }
    m_modbus = Create_modbus_rtu();
    connect(m_modbus, &QModbusClient::errorOccurred, [&](QModbusDevice::Error error) {
        Q_UNUSED(error);
        qWarning() << m_modbus->error() << m_modbus->errorString();
        if (m_modbus->error() == QModbusDevice::ConnectionError) {
            m_modbus->disconnectDevice();
            qWarning() << "disconnect finish";
        }
    });
    /// [modbus-init]

    while (th_isRunning) {
        task_type task{};
        {
            std::unique_lock<std::mutex> lock(th_taskMutex);
            while (m_taskQueue.size() == 0 && th_isRunning) {
                th_taskCV.wait(lock);
            }
            if (th_isRunning == false) {
                break;
            }
            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
        }
        if (task) {
            QThread::msleep(m_timeTaskInterval);
            task();
        }
    }

    /// 这里是否需要清理任务队列，需决议
    if (m_modbus) {
        if (m_modbus->state() == QModbusDevice::ConnectedState) {
            m_modbus->disconnectDevice();
        }
        m_modbus->deleteLater();
        m_modbus = nullptr;
    }
}

}  // namespace COM

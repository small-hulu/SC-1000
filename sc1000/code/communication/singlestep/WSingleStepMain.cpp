#include "WSingleStepMain.h"

#include <QDateTime>
#include <QDebug>
#include <QModbusClient>
#include <QModbusDevice>
#include <QModbusReply>
#include <QModbusRtuSerialMaster>
#include <QSerialPort>
#include <QSerialPortInfo>

#include "WMultStpeTrigger.h"
#include "WSSFactory.h"
#include "communication/COM_Device.hpp"
#include "communication/controller/Init.h"
#include "communication/device/ModbusImpl.h"
#include "communication/utility/ToString.h"
#include "ui_WSingleStepMain.h"

namespace COM::SINGLEWSTEP {
SingleStepMain::SingleStepMain(QWidget *parent) : QMainWindow(parent), ui(new Ui::SingleStepMain) {
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(QString("单步操作") + qApp->applicationVersion());

    /// test-ui
    {
        /// 单流程 & 多流程
        new MultStpeTrigger{ui->widget_test};
    }

    /// ui tablist
    {
        auto tab = ui->tabWidget;
        while (tab->count()) {
            auto widget = tab->widget(0);
            tab->removeTab(0);
            widget->deleteLater();
        }

        auto widgetList = Create_ssWidgetList(ui->tabWidget);
        for (auto [name, widget] : widgetList) {
            tab->addTab(widget, name);
        }
    }

    /// connect
    {
        /// 串口重新扫描
        connect(ui->btn_port, &QPushButton::clicked, this, &SingleStepMain::refresh_port);

        connect(ui->btn_open, &QPushButton::clicked, this, &SingleStepMain::open_modbus);
        connect(ui->btn_read, &QPushButton::clicked, this, &SingleStepMain::read_modbus);
        connect(ui->btn_write, &QPushButton::clicked, this, &SingleStepMain::write_modbus);
        connect(ui->btn_todo, &QPushButton::clicked, this, []() {
            qDebug().noquote().nospace() << COM::DeviceManager::to_string();
            qDebug().noquote().nospace() << COM::DeviceOpAuthorizeCU::instance().to_string();
        });
    }

    /// init
    {
        refresh_port();
        /// 启动自动轮询操作
        // m_timeId = startTimer(1000);
    }

    qInfo() << __func__;
}

SingleStepMain::~SingleStepMain() {
    qCritical() << __func__;
    delete ui;
}

/**
 * 刷新串口号
 */
void SingleStepMain::refresh_port() {
    ui->combo_serialport->clear();
    for (auto &info : QSerialPortInfo::availablePorts()) {
        ui->combo_serialport->addItem(info.portName());
    }
}

/**
 * 连接 modbus
 * 以RTU的Serial进行连接
 */
void SingleStepMain::open_modbus() {
    const QString port = ui->combo_serialport->currentText();
    COM::Init(port);
}

/**
 * 读取数据
 */
void SingleStepMain::read_modbus() {
    int     startAddress  = ui->edit_startAddress->text().toInt();
    quint16 u16Length     = ui->edit_dateLength->text().toUShort();
    int     serverAddress = ui->edit_deviceAddress->text().toInt();

    /// - 何种寄存器
    /// - 起始位置
    /// - 数据长度
    QModbusDataUnit data(QModbusDataUnit::HoldingRegisters, startAddress, u16Length);

    /// 数据
    /// 设备地址
    data = COM::ModbusImpl::instance().read(serverAddress, data).data;
    qDebug().noquote() << COM::to_string(data);
}

/**
 * 写入数据
 * 只能写进保持寄存器 HoldingRegisters
 */
void SingleStepMain::write_modbus() {
    int startAddress  = ui->edit_startAddress->text().toInt();
    int serverAddress = ui->edit_deviceAddress->text().toInt();

    QModbusDataUnit data(QModbusDataUnit::HoldingRegisters, startAddress, 0);

    QStringList       txt = ui->textEdit->toPlainText().split(" ");
    QVector<uint16_t> u16Values;
    for (auto &&s : txt) {
        uint16_t u16 = s.toShort(nullptr, 16);
        u16Values.push_back(u16);
    }
    data.setValues(u16Values);

    qDebug().noquote() << ">>>自定义写入的数据为：" << COM::to_string(data);
    data = COM::ModbusImpl::instance().write(serverAddress, data).data;
    qDebug().noquote() << COM::to_string(data);
}

/**
 * 为辅助测试的自动轮询操作函数
 */
void SingleStepMain::timerEvent(QTimerEvent *event) {
    QMainWindow::timerEvent(event);
}
}  // namespace COM::SINGLEWSTEP

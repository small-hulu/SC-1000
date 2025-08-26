#include <QApplication>
#include <QFileDialog>
#include <QMetaEnum>
#include <QTextCursor>

#include "code/MCUUpgrade.h"
#include "communication/COM_Device.hpp"
#include "communication/bootloader/bootloader_operation.h"
#include "communication/controller/Init.h"
#include "communication/device/gripper/DGripper.h"
#include "log/LOG.h"

bool modbus_init(int vid, int pid) {
    using namespace COM;
    auto modbusPtr = &COM::ModbusImpl::instance();
    if (modbusPtr->isOpen() == true) {
        qInfo() << "modbus already opened";
        return true;
    }

    ModbusImpl::Info info;
    info.vid = vid;
    info.pid = pid;
    for (QSerialPortInfo serialInfo : QSerialPortInfo::availablePorts()) {
        if (!(serialInfo.vendorIdentifier() == info.vid &&
              serialInfo.productIdentifier() == info.pid)) {
            continue;
        }
        info.port = serialInfo.portName();
        modbusPtr->Set_info(info);
        if (modbusPtr->open() == false) {
            qWarning().noquote().nospace() << to_string(serialInfo) << "modbus connet=" << false;
            return false;
        } else {
            qInfo().noquote().nospace() << to_string(serialInfo) << "modbus connect=" << true;
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<QTextCursor>("QTextCursor");

    qInstallMessageHandler(LOG::simple_messageHandler);
    LOG::SLog::Init_console(true);

    if (modbus_init(0x1A86, 0x55d3) == false) {
        qWarning() << ">>> 连接 False" << QString("vid[%1] pid[%2]").arg(0x1A86).arg(0x55d3);
        // return EXIT_FAILURE;
    } else {
        qInfo() << ">>> 连接 True" << QString("vid[%1] pid[%2]").arg(0x1A86).arg(0x55d3);
    }

    MCUUpgrade update;
    update.show();
    update.setWindowTitle("下位机升级程序");

    return app.exec();
}

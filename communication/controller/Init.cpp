#include "Init.h"

#include "../COM_Device.hpp"
#include "../controller/WatchDog.h"
#include "../utility/IniConfig.h"
#include "ActionMajor.h"
#include "DeviceOpAuthorizeCU.h"
#include "Event.h"
#include "communication/bootloader/bootloader_operation.h"

namespace COM {
bool Init(std::optional<QString> portName) {
    Init_instance();

    auto modbusPtr = &COM::ModbusImpl::instance();
    if (modbusPtr->isOpen() == true) {
        qInfo() << "modbus already opened";
        return true;
    }
    if (portName.has_value() == false) {
        auto        &&ini = IniConfig::instance();
        const quint16 vid = ini[KI::modbus_serial_vid].toInt();
        const quint16 pid = ini[KI::modbus_serial_pid].toInt();
        for (QSerialPortInfo serialInfo : QSerialPortInfo::availablePorts()) {
            if (!(serialInfo.vendorIdentifier() == vid && serialInfo.productIdentifier() == pid)) {
                continue;
            }
            portName = serialInfo.portName();
            break;
        }
    }
    if (portName.has_value()) {
        QString msg = QString("modbus open by[%1] is: ").arg(portName.value());
        qInfo() << msg << modbusPtr->open(portName.value());
    }
    if (modbusPtr->isOpen() == false) {
        return false;
    }

    Init_bootloader();

    auto &&ini = IniConfig::instance();
    if (ini[KI::enable_initMCU].toUInt()) {
        Init_mcu();
    }
    if (ini[KI::enable_loopEachDevice].toInt()) {
        Start_allLoops();
    }

#ifdef QT_NO_DEBUG
    Log_message();
#endif
    return modbusPtr->isOpen();
}

bool Init_instance() {
    IniConfig::instance();

    ModbusImpl::instance();

    Event::instance();

    DGripper::instance();
    DReagentBin::instance();
    DMagneticSeparation::instance();
    DPipetteNeedle::instance();
    DMixing::instance();
    DPhotoelectric::instance();
    DThermostat::instance();

    WatchDog::instance();

    DeviceOpAuthorizeCU::instance();

    return true;
}

void Init_bootloader() {
    ModbusImpl::Info info;
    auto           &&ini    = IniConfig::instance();
    bool             isBoot = ini[KI::bootloader_enable].toInt();
    if (isBoot == false) {
        qInfo() << "ini-set not check bootloader";
        return;
    } else {
        qInfo() << "ini-set check bootloader";
    }

    QList<int> deviceAddress;
    deviceAddress << DeviceAddress_ReagentBin;
    deviceAddress << DeviceAddress_PipettingNeedle;
    deviceAddress << DeviceAddress_Gripper;
    deviceAddress << DeviceAddress_MagneticSeparation;
    deviceAddress << DeviceAddress_Mixing;
    deviceAddress << DeviceAddress_Photoelectric;
    deviceAddress << DeviceAddress_Thermostat;

    QString msg;
    for (int add : deviceAddress) {
        BOOTLOADER::BootloaderOp boot(add);
        /// 是在boot中才需要jump to app
        if (boot.check_is_bootloader() == 1) {
            int flag  = boot.goto_app();
            msg      += QString::asprintf("Add[%d] not in boot into[%d]", add, flag);
            msg      += "\n";
        } else {
            msg += QString::asprintf("Add[%d] in app", add);
            msg += "\n";
        }
    }

    qInfo().noquote() << msg;
}

void Init_mcu() {
    DGripper::instance().Init_muc();
    DReagentBin::instance().Init_muc();
    DMagneticSeparation::instance().Init_muc();
    DPipetteNeedle::instance().Init_muc();
    DMixing::instance().Init_muc();
    DPhotoelectric::instance().Init_muc();
    DThermostat::instance().Init_muc();
}

void Start_allLoops() {
    DGripper::instance().Start_thread();
    DReagentBin::instance().Start_thread();
    DMagneticSeparation::instance().Start_thread();  //IDeviceThread start here
    DPipetteNeedle::instance().Start_thread();
    DMixing::instance().Start_thread();
    DPhotoelectric::instance().Start_thread();
    DThermostat::instance().Start_thread();

    WatchDog::instance().Start_thread();
}

void Stop_allLoops() {
    WatchDog::instance().Stop_thread();

    DGripper::instance().Stop_thread();
    DReagentBin::instance().Stop_thread();
    DMagneticSeparation::instance().Stop_thread();
    DPipetteNeedle::instance().Stop_thread();
    DMixing::instance().Stop_thread();
    DPhotoelectric::instance().Stop_thread();
    DThermostat::instance().Stop_thread();
}

void Log_message() {
    auto &ini = IniConfig::instance();
    qInfo().noquote().nospace() << ini.to_string();
}

/**
 * 需要一个一个检查
 */
bool Exit() {
    Stop_allLoops();

    auto modbusPtr = &COM::ModbusImpl::instance();
    if (modbusPtr->isOpen()) {
        /// 退出前设置为黑色，表示不受控制
        DReagentBin::instance().Set_LED_RGB(0, 0, 0);
        modbusPtr->close();
    }

    DeviceOpAuthorizeCU::instance().Stop_thread();
    return false;
}

}  // namespace COM

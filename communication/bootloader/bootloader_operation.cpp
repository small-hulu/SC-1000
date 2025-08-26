/** ***************************************************************************
 * Copyright © 2024 zhoudeyang
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       bootloader_operation.cpp
 * @author     zhoudeyang
 * @Email      zhoudeyang@smtbio.com
 * @date       2024年8月13日
 * @version    0.0.1
 *
 * @brief      bootloader operatoin
 *
 *****************************************************************************/

// #define QT_NO_DEBUG_OUTPUT

#include "bootloader_operation.h"

#include <QDateTime>
#include <QFile>
#include <QVector>
#include <cstring>
#include <memory>

#include "bootloader_register.hpp"
#include "communication/device/ModbusImpl.h"

namespace COM::BOOTLOADER {

BootloaderOp::BootloaderOp(int slave_address, QObject* parent) : QObject(parent) {
    // set address
    this->m_address = slave_address;
}

int BootloaderOp::rising_event_send(int reg_address, int bit) {
    if ((bit > 15) || bit < 0) {
        return ErrParm;
    }
    // get device
    auto&&          device = ModbusImpl::instance();
    QModbusDataUnit data(RegType, reg_address, 1);
    // read control register
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    // set low
    uint16_t tmp = ret.data.value(0) & (~(0x01 << bit));
    data.setValue(0, tmp);
    ret = device.write(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    // wait 20 ms
    QThread::msleep(20);
    // set high & sent rising event
    tmp = tmp | (0x01 << bit);
    data.setValue(0, tmp);
    ret = device.write(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "write failed";
        return ErrCom;
    }
    // succeed
    return ErrSucceed;
}

int BootloaderOp::rising_event_send_whitout_reply(int reg_address, int bit) {
    if ((bit > 15) || bit < 0) {
        return ErrParm;
    }
    // get device
    auto&&          device = ModbusImpl::instance();
    QModbusDataUnit data(RegType, reg_address, 1);
    // read control register
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    // set low
    uint16_t tmp = ret.data.value(0) & (~(0x01 << bit));
    data.setValue(0, tmp);
    ret = device.write(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "write failed";
        return ErrCom;
    }
    // wait 20 ms
    QThread::msleep(20);
    // set high & sent rising event
    tmp = tmp | (0x01 << bit);
    data.setValue(0, tmp);
    // dont check modbus reposne, lower may not maintaining modbus
    ret = device.write(this->m_address, data);

    // succeed
    return ErrSucceed;
}

int BootloaderOp::check_is_bootloader() {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_SYS_INFO, 4);
    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    // 0 is in bootloader, 1 not in bootloader
    return !(ret.data.value(0) & 0x0001);
}

int BootloaderOp::system_reset() {
    return rising_event_send_whitout_reply(Bootloader_REG_SYS_RESET, 0);
}

int BootloaderOp::system_reset(uint64_t timeout) {
    int ret = 0;
    // send reset event
    ret     = rising_event_send_whitout_reply(Bootloader_REG_SYS_RESET, 0);
    if (ret) {
        return ret;
    }

    // wait
    QThread::msleep(100);
    // get timestamp
    uint64_t timestamp = QDateTime::currentMSecsSinceEpoch();
Loop:
    // check if in bootloader
    ret = check_is_bootloader();
    if (ret != 1) {
        // not in bootloader or commciation error/no response
        if (QDateTime::currentMSecsSinceEpoch() - timestamp > timeout) {
            return ErrTimeout;
        }
        goto Loop;
    } else {
        return ErrSucceed;
    }
}

int BootloaderOp::check_op_done() {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_STATUS, 1);
    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    return !!(ret.data.value(1) & 0x08);
}

int BootloaderOp::wait_op_done(uint64_t timeout) {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_STATUS, 1);

    uint64_t timestamp = QDateTime::currentMSecsSinceEpoch();
Loop:
    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }

    if ((ret.data.value(0) & 0x08) == 0) {
        // not done

        if (QDateTime::currentMSecsSinceEpoch() - timestamp > timeout) {
            return ErrTimeout;
        }
        // not done, msleep 100 ms
        QThread::msleep(100);
        goto Loop;

    } else {
        return ErrSucceed;
    }
}

int BootloaderOp::app_pack_version_set(uint32_t version) {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_VERSION, 2);
    data.setValue(0, (uint16_t)(version & 0x0000FFFF));
    data.setValue(1, (uint16_t)((version >> 16) & 0x0000FFFF));
    // read
    ModbusReplyWrapper ret = device.write(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    return ErrSucceed;
}

int BootloaderOp::app_pack_timestamp_set(uint32_t timestamp) {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_TIMESTAMP, 2);
    data.setValue(0, (uint16_t)(timestamp & 0x0000FFFF));
    data.setValue(1, (uint16_t)((timestamp >> 16) & 0x0000FFFF));
    // read
    ModbusReplyWrapper ret = device.write(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "write failed";
        return ErrCom;
    }
    return ErrSucceed;
}

int BootloaderOp::bootloader_status_get() {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_STATUS, 1);

    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }

    return !!(ret.data.value(0) & 0x07);
}

int BootloaderOp::bootloader_version_get(uint32_t& version) {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_VER_MAJOR, 3);

    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }

    version = ((ret.data.value(0) << 16) & 0x00FF0000) | ((ret.data.value(1) << 8) & 0x0000FF00) |
              ((ret.data.value(2) << 0) & 0x000000FF);
    return ErrSucceed;
}

int BootloaderOp::cur_app_timestamp_get(uint32_t& timestamp) {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_CUR_TIMESTAMP, 2);

    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }

    timestamp = ret.data.value(0) | (ret.data.value(1) << 16);
    return ErrSucceed;
}

int BootloaderOp::cur_app_version_get(uint32_t& version) {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_CUR_VERSION, 2);

    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    // get version
    version = ret.data.value(0) | (ret.data.value(1) << 16);
    return ErrSucceed;
}

int BootloaderOp::pack_index_get() {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_INDEX, 1);

    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    return ret.data.value(0);
}

int BootloaderOp::op_res_get() {
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data(RegType, Bootloader_REG_STATUS, 1);

    // read
    ModbusReplyWrapper ret = device.read(this->m_address, data);
    if (ret.is_valid() == false) {
        qDebug() << "read failed";
        return ErrCom;
    }
    if (ret.data.value(0) & 0x10) {
        return ErrSucceed;
    } else if (ret.data.value(0) & 0x20) {
        return ErrOther;
    } else {
        return ErrNoResult;
    }
}

int BootloaderOp::flash_erase(uint64_t timeout) {
    int ret = 0;
    ret     = bootloader_status_get();
    if (ret != 1) {
        ret = (ret == 0) ? ErrStatus : ret;
        return ret;
    }

    // send event
    ret = rising_event_send(Bootloader_REG_CTRL, 2);
    if (ret) {
        return ret;
    }
    // wait erase  about 3s
    QThread::msleep(3000);
    // wait done
    ret = wait_op_done(timeout);
    if (ret) {
        // error
        return ret;
    }
    // get result
    return op_res_get();
}

int BootloaderOp::pack_flash_write(uint64_t timeout) {
    int ret = 0;
    ret     = bootloader_status_get();
    if (ret != 1) {
        ret = (ret == 0) ? ErrStatus : ret;
        return ret;
    }

    // send event
    ret = rising_event_send(Bootloader_REG_CTRL, 3);
    if (ret) {
        return ret;
    }
    // wait erase  about 50mx
    QThread::msleep(50);
    // wait done
    ret = wait_op_done(timeout);
    if (ret) {
        // error
        return ret;
    }
    // get result
    return op_res_get();
}

int BootloaderOp::trans_finish(uint64_t timeout) {
    int ret = 0;
    ret     = bootloader_status_get();
    if (ret != 1) {
        ret = (ret == 0) ? ErrStatus : ret;
        return ret;
    }
    // send event
    ret = rising_event_send(Bootloader_REG_CTRL, 4);
    if (ret) {
        return ret;
    }
    // wait erase  about 50ms
    QThread::msleep(50);
    // wait done
    ret = wait_op_done(timeout);
    if (ret) {
        // error
        return ret;
    }
    // get result
    return op_res_get();
}

int BootloaderOp::write_pack_data(char* data, uint32_t length) {
    if (length != 1024) {
        // parm error
        return ErrParm;
    }
    // data length must 1024 byte
    auto&& device = ModbusImpl::instance();
    // creat data unit
    QModbusDataUnit data_unit(RegType, Bootloader_REG_DATA, 64);

    QVector<uint16_t> vec(64);

    for (int i = 0; i < 8; i++) {
        // set address
        data_unit.setStartAddress(i * 64 + Bootloader_REG_DATA);

        memcpy(&vec[0], (uint16_t*)data + i * 64, 128);
        data_unit.setValues(vec);

        // read
        ModbusReplyWrapper ret = device.write(this->m_address, data_unit);
        // 8 trans ,each transfer 128 Byte/64 register data
        if (ret.is_valid() == false) {
            qDebug() << "wirte failed";
            return ErrCom;
        }
    }

    return ErrSucceed;
}

int BootloaderOp::goto_app(uint64_t timeout) {
    // check if in idle state
    int ret = 0;
    ret     = bootloader_status_get();
    if (ret != 0) {
        // not in idle state
        ret = (ret == 1) ? ErrStatus : ret;
        return ret;
    }

    // send event,dont check ret
    ret = rising_event_send_whitout_reply(Bootloader_REG_CTRL, 0);
    if (ret) {
        return ret;
    }
    // wait 100 ms
    QThread::msleep(100);
    // get timestamp
    uint64_t timestamp = QDateTime::currentMSecsSinceEpoch();
Loop:
    // check is in app
    ret = check_is_bootloader();
    if (ret != 0) {
        if (QDateTime::currentMSecsSinceEpoch() - timestamp > timeout) {
            // timeout
            return ErrTimeout;
        }
        // not in app or com error
        QThread::msleep(100);
        goto Loop;

    } else {
        return ErrSucceed;
    }
    // never run here
    qDebug() << "error ";
    return ErrAssertion;
}

int BootloaderOp::goto_upgrade(uint64_t timeout) {
    // check if in idle state
    int ret = 0;
    ret     = bootloader_status_get();
    if (ret != 0) {
        // not in idle state
        ret = (ret == 1) ? ErrStatus : ret;
        return ret;
    }
    // send event,dont check ret
    ret = rising_event_send(Bootloader_REG_CTRL, 1);
    if (ret) {
        return ret;
    }

    // get timestamp
    uint64_t timestamp = QDateTime::currentMSecsSinceEpoch();
Loop:
    ret = bootloader_status_get();

    if (ret != 1) {
        if (QDateTime::currentMSecsSinceEpoch() - timestamp > timeout) {
            // timeout
            return ErrTimeout;
        }
        goto Loop;

    } else {
        return ErrSucceed;
    }
}

int BootloaderOp::goto_idle(uint64_t timeout) {
    // check if in upgrade state
    int ret = 0;
    ret     = bootloader_status_get();
    if (ret != 1) {
        // not in idle state
        ret = (ret == 0) ? ErrStatus : ret;
        return ret;
    }
    // send event,dont check ret
    ret = rising_event_send(Bootloader_REG_CTRL, 5);
    if (ret) {
        return ret;
    }

    // get timestamp
    uint64_t timestamp = QDateTime::currentMSecsSinceEpoch();
Loop:
    ret = bootloader_status_get();

    if (ret != 0) {
        if (QDateTime::currentMSecsSinceEpoch() - timestamp > timeout) {
            // timeout
            return ErrTimeout;
        }
        goto Loop;

    } else {
        return ErrSucceed;
    }
}

BootloaderOp::ErrUpgrade BootloaderOp::device_upgrade(const QString& filePath,
                                                      uint32_t       new_app_version,
                                                      UpgradeState&  state) {
    int ret = 0;
    qDebug() << "start reset target device system";
    state.state = UpgradeState::StateResetSystem;
    // system reset first
    ret         = system_reset(1000);
    if (ret) {
        // operation error
        return ErrSystemReset;
    }
    qDebug() << "reset done";
    state.state = UpgradeState::StateGetInfo;
    // get bootloader version
    uint32_t bl_version;
    ret = bootloader_version_get(bl_version);
    if (ret) {
        return ErrBootloaderVersionGet;
    }

    // print bootloader version
    qDebug() << "bootloader version is : " << bl_version;
    // get cur app version and timestamp
    uint32_t app_version;
    uint32_t app_timestamp;
    ret = cur_app_timestamp_get(app_timestamp);
    if (ret) {
        return ErrAppTimestampGet;
    }
    ret = cur_app_version_get(app_version);
    if (ret) {
        return ErrAppVersionGet;
    }
    // print app version and timestamp
    qDebug() << "app version is : " << app_version;
    qDebug() << "app timestamp is : " << app_timestamp;

    state.state = UpgradeState::StatePrepareDownload;

    // now in bootloader and in idle status
    // goto upgrade state
    ret = goto_upgrade(1000);
    if (ret < 0) {
        return ErrStartUpgrade;
    }
    state.state = UpgradeState::StateEraseFlash;
    // wait for erase flash
    qDebug() << "wait for erase flash";
    // erase flash , timeout 5s
    ret = flash_erase(5000);
    if (ret) {
        return ErrEraseFlash;
    }
    qDebug() << "erase flash done";

    state.state = UpgradeState::StateStartDownload;
    // now in upgrede state
    qDebug() << "wait for downlaod firmware";
    // open file
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "read fille error";
        return ErrOpenFile;
    }
    // get file size
    uint32_t file_pack_count = (file.size() + 1023) / 1024;
    qDebug() << "new file size is " << (double)file.size() / 1024 << "KBytes";

    uint16_t host_index = 0;
    // allocate data buff
    std::unique_ptr<char[]> data_buff(new char[1024]);

    state.state = UpgradeState::StateDownloading;
    while (1) {
        // get pack_index
        int pack_index = pack_index_get();
        if (pack_index < 0) {
            return ErrPackIndexGet;
        }
        if (host_index != pack_index) {
            return ErrPackIndexNotMatch;
        }
        // read data to data buffer
        uint64_t read_len = file.read(data_buff.get(), 1024);
        if (read_len != 1024) {
            // last pack
            // set 0 to pack end
            memset(data_buff.get() + read_len, 0xFF, 1024 - read_len);
        }
        // write data
        ret = write_pack_data(data_buff.get(), 1024);
        if (ret) {
            return ErrPackDataWrite;
        }
        // write pack to flash, timeout 1s
        ret = pack_flash_write(1000);
        if (ret) {
            return ErrFlashWrite;
        }
        // update pre and host index increase
        host_index++;
        state.pre = (double)host_index / file_pack_count;
        // print pre
        qDebug() << state.pre * 100 << "%";

        if (read_len != 1024) {
            // last pack
            // write app verison
            ret = app_pack_version_set(new_app_version);
            if (ret) {
                return ErrWriteVersion;
            }

            ret = app_pack_timestamp_set((uint32_t)QDateTime::currentSecsSinceEpoch());
            if (ret) {
                return ErrWriteTimestamp;
            }
            // end finish event,timeout 1s
            ret = trans_finish(1000);
            if (ret) {
                return ErrTransFinish;
            }
            // exit loop
            break;
        }
    }
    state.state = UpgradeState::StateJummpingToApp;
    // download ok
    // to go idle ,timeout 1s
    ret         = goto_idle(1000);
    if (ret) {
        return ErrBackIdle;
    }
    // goto app
    ret = goto_app(1000);
    if (ret) {
        return ErrGotoApp;
    }
    state.state = UpgradeState::StateDone;

    return ErrUpdateSucceed;
}
}  // namespace COM::BOOTLOADER

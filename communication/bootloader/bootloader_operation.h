/** ***************************************************************************
 * Copyright © 2024 zhoudeyang
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       bootloader_operation.h
 * @author     zhoudeyang
 * @Email      zhoudeyang@smtbio.com
 * @date       2024年8月13日
 * @version    0.0.1
 *
 * @brief      bootloader operatoin
 *
 *****************************************************************************/
#ifndef __BOOTLOADER_OPERATION_H__
#define __BOOTLOADER_OPERATION_H__

#include <QModbusDataUnit>
#include <QObject>
#include <QString>
#include <cstdint>

namespace COM::BOOTLOADER {
class BootloaderOp : public QObject {
    Q_OBJECT

public:
    enum ErrOp : int {
        ErrOther     = 1,
        ErrSucceed   = 0,
        ErrTimeout   = -1,
        ErrParm      = -2,
        ErrStatus    = -3,
        ErrCom       = -4,
        ErrNoResult  = -5,
        ErrAssertion = -6,
    };
    Q_ENUM(ErrOp);

    enum ErrUpgrade : int {
        ErrUpdateSucceed = 0,
        ErrOpenFile,
        ErrBootloaderVersionGet,
        ErrSystemReset,
        ErrStartUpgrade,
        ErrEraseFlash,
        ErrAppTimestampGet,
        ErrAppVersionGet,
        ErrPackIndexGet,
        ErrPackIndexNotMatch,
        ErrPackDataWrite,
        ErrFlashWrite,
        ErrWriteVersion,
        ErrWriteTimestamp,
        ErrTransFinish,
        ErrBackIdle,
        ErrGotoApp,
    };
    Q_ENUM(ErrUpgrade);

    struct UpgradeState {
        enum State : int {
            StateInvalid = 0,
            StateResetSystem,
            StateGetInfo,
            StatePrepareDownload,
            StateEraseFlash,
            StateStartDownload,
            StateDownloading,
            StateJummpingToApp,
            StateDone,
        };
        State  state = StateInvalid;
        double pre   = 0;
    };

private:
    const QModbusDataUnit::RegisterType RegType   = QModbusDataUnit::RegisterType::HoldingRegisters;
    int                                 m_address = 10;

public:
    BootloaderOp(int slave_address, QObject *parent = nullptr);

public:
    /**
     * @brief check now is in bootloader
     * @return
     * ret<0, wrong code
     * 0 = not in bootloader
     * 1 = in bootloader
     */
    int check_is_bootloader();

    /**
     * @brief reset system
     * @note  is function may gank modbus communication
     *        system reset will not maintaining modbus
     *        communication may no respone
     * @return
     * ret<0, wrong code
     * 0 = reset ok
     */
    int system_reset();

    /**
     * @brief reset system
     * @note  is function may gank modbus communication
     *        system reset will not maintaining modbus
     *        communication may no respone
     * @return
     * ret<0, wrong code
     * 0 = reset ok
     */
    int system_reset(uint64_t timeout);

    /**
     * @brief get bootloader global status
     * @return
     * ret<0, wrong code
     * 0 = idle state
     * 1 = upgrade state
     */
    int bootloader_status_get();

    /**
     * @brief get bootloader version
     * @return
     * ret<0, wrong code
     * 0 =  get succeed
     */
    int bootloader_version_get(uint32_t &version);

    /**
     * @brief current app timestamp get
     * @return
     * ret<0, wrong code
     *
     * 0,get succeed
     */
    int cur_app_timestamp_get(uint32_t &timestamp);

    /**
     * @brief current app version get
     * @return
     * ret<0, wrong code
     *
     * 0, get succeed
     */
    int cur_app_version_get(uint32_t &version);

    /**
     * @brief jump to app
     * @note  jump to app will not maintaining modbus
     *        communication may no respone
     *        wait app enter.
     * @return
     * ret<0, wrong code
     * -1 = operation timeout
     * -3 =  state error, now in bootloader idle status
     * 0 = operation succeed
     */
    int goto_app(uint64_t = 5000);

    /**
     * @brief upgrede target device
     * @note  Will block the thread
     *
     * @return  UpGradeState
     */
    ErrUpgrade device_upgrade(const QString &filePath, uint32_t new_app_version,
                              UpgradeState &state);

private:
    /**
     * @brief send a bit rising event
     * @return
     * ret<0, wrong code
     * 0 = operation succeed
     */
    int rising_event_send(int reg_address, int bit);

    /**
     * @brief send a bit rising event
     * @return
     * ret<0, wrong code
     * 0 = operation succeed
     */
    int rising_event_send_whitout_reply(int reg_address, int bit);

    /**
     * @brief check operation is done
     *        will return immediately
     * @return
     * ret<0, wrong code
     * 0 = not done
     * 1 = done
     */
    int check_op_done();

    /**
     * @brief goto upgreade status
     *        and wait until in upgrade status
     * @return
     * ret<0, wrong code
     * -1 = operation timeout
     * 0 = operation succeed
     */
    int goto_upgrade(uint64_t timeout);

    /**
     * @brief goto idle status
     *        and wait until in upgrade status
     * @return
     * ret<0, wrong code
     * -1 = operation timeout
     * 0 = operation succeed
     */
    int goto_idle(uint64_t timeout);

    /**
     * @brief current pack index get
     * @return
     * ret<0, wrong code
     * else index
     */
    int pack_index_get();

    /**
     * @brief last operation result get
     * @return
     * ret<0, wrong code
     * 1 = operation failed
     * 0 = operation succeed
     */
    int op_res_get();

    /**
     * @brief erase flash and wait earse done
     * @return
     * ret<0, wrong code
     * -1 = operation timeout
     * 0 = operation succeed
     */
    int flash_erase(uint64_t timeout);

    /**
     * @brief wait pack data to flash and wait write done
     * @return
     * ret<0, wrong code
     * -1 = operation timeout
     * 0 = operation succeed
     */
    int pack_flash_write(uint64_t timeout);

    /**
     * @brief wait  magic to flash and finish this trans
     *        and wait write done
     * @return
     * ret<0, wrong code
     * -1 = operation timeout
     * 0 = operation succeed
     */
    int trans_finish(uint64_t timeout);

    /**
     * @brief trans data to pack data register
     *
     * @par   data length must be 1024 bytes
     * @return
     * ret<0, wrong code
     * -1 = operation timeout
     * 0 = operation succeed
     */
    int write_pack_data(char *data, uint32_t length);

    /**
     * @brief wait until operation done until timout
     * @return
     * ret<0, wrong code
     * -1 = timeout
     * 0 = done
     */
    int wait_op_done(uint64_t timeout);

    /**
     * @brief write app version
     * @return
     * ret<0, wrong code
     * 0 = done
     */
    int app_pack_version_set(uint32_t version);

    /**
     * @brief write app timestamp
     * @return
     * ret<0, wrong code
     * 0 = done
     */
    int app_pack_timestamp_set(uint32_t timestamp);
};
}  // namespace COM::BOOTLOADER

#endif

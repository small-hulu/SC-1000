/** ***************************************************************************
 * Copyright © 2024 zhoudeyang
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       bootloader_register.h
 * @author     zhoudeyang
 * @Email      zhoudeyang@smtbio.com
 * @date       2024年8月13日
 * @version    0.0.1
 *
 * @brief      modbus resigter define
 *
 *****************************************************************************/
#ifndef __BOOTLOADER_REGISTER_H__
#define __BOOTLOADER_REGISTER_H__

namespace COM::BOOTLOADER {
// control register
constexpr int Bootloader_REG_CTRL          = 4000;
// status register
constexpr int Bootloader_REG_STATUS        = 4001;
// CRC register
constexpr int Bootloader_REG_CRC           = 4002;
// pack index register
constexpr int Bootloader_REG_INDEX         = 4004;
// timestamp register
constexpr int Bootloader_REG_TIMESTAMP     = 4006;
// version register
constexpr int Bootloader_REG_VERSION       = 4008;
// current version register
constexpr int Bootloader_REG_CUR_VERSION   = 4010;
// current timestamp register
constexpr int Bootloader_REG_CUR_TIMESTAMP = 4012;
// data register
constexpr int Bootloader_REG_DATA          = 5000;
// firmware info register
constexpr int Bootloader_REG_SYS_INFO      = 0;
// firmware version major
constexpr int Bootloader_REG_VER_MAJOR     = 1;
// firmware version minor
constexpr int Bootloader_REG_VER_MINOR     = 2;
// firmware version patch
constexpr int Bootloader_REG_VER_PATCH     = 3;
// system control register
constexpr int Bootloader_REG_SYS_RESET     = 2000;
}  // namespace COM::BOOTLOADER

#endif

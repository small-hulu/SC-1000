/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       Config_CMixing.h
 * @date       2024-03-07
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      分杯混匀模块的state
 *
 *****************************************************************************/
#ifndef CMIXING_H_1709778830
#define CMIXING_H_1709778830

#include <QString>

#include "../../config/CLteErrorCode.hpp"

namespace COM {
struct CMixing {
    struct Params {
        bool isValid = false;

        int cupTarPosition = 0;
        int cupInPosition  = -1;
        int cupOutPosition = -1;

        QString to_string() const;
    };

    struct StateBits {
        bool isValid = false;

        /// 混匀状态[1]
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_hoppeFull : 1;            // 00-料斗非空
                uint8_t b01_slideFull : 1;            // 01-滑道非空
                uint8_t b02_cupholeFull : 1;          // 02-杯孔非空
                uint8_t : 5;                          // [占位]
                uint8_t b08_wasteLiquidTankFull : 1;  // 08-废液桶
                uint8_t : 7;                          // [占位]
            };
        } u1000_Mixing;
        static_assert(sizeof(u1000_Mixing) == sizeof(uint16_t), "The Bits Are Not Aligned");

        /// 混匀状态呀[2]
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_selfCheck : 1;  // 00-自检完成标志
                uint8_t : 7;                // [占位]
                uint8_t : 8;                // [占位]
            };
        } u1001_Mixing;
        static_assert(sizeof(u1001_Mixing) == sizeof(uint16_t), "The Bits Are Not Aligned");

        /// 上料模块
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_leisure : 1;           // 00-动作空闲 （1空闲，0非空闲）
                uint8_t : 7;                       // [占位]
                uint8_t b08_ErrorAssembly : 1;     // 08-部件错误
                uint8_t : 4;                       // [占位]
                uint8_t b13_ErrorRunTimeOut : 1;   // 13-动作超时
                uint8_t b14_ErrorDrive : 1;        // 14-驱动器错误
                uint8_t b15_ErrorLockedRotor : 1;  // 15-电机堵转
            };
        } u1003_LoadMaterial_LMoto;
        static_assert(sizeof(u1003_LoadMaterial_LMoto) == sizeof(uint16_t),
                      "The Bits Are Not Aligned");

        /// 分杯模块
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_leisure : 1;           // 00-动作空闲 （1空闲，0非空闲）
                uint8_t : 7;                       // [占位]
                uint8_t b08_ErrorAssembly : 1;     // 08-部件错误
                uint8_t : 4;                       // [占位]
                uint8_t b13_ErrorRunTimeOut : 1;   // 13-动作超时
                uint8_t b14_ErrorDrive : 1;        // 14-驱动器错误
                uint8_t b15_ErrorLockedRotor : 1;  // 15-电机堵转
            };
        } u1005_DivideCup_PMoto;
        static_assert(sizeof(u1005_DivideCup_PMoto) == sizeof(uint16_t),
                      "The Bits Are Not Aligned");

        /// 混匀模块
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_leisure : 1;           // 00-动作空闲 （1空闲，0非空闲）
                uint8_t : 7;                       // [占位]
                uint8_t b08_ErrorAssembly : 1;     // 08-部件错误
                uint8_t : 4;                       // [占位]
                uint8_t b13_ErrorRunTimeOut : 1;   // 13-动作超时
                uint8_t b14_ErrorDrive : 1;        // 14-驱动器错误
                uint8_t b15_ErrorLockedRotor : 1;  // 15-电机堵转
            };
        } u1007_Mix_VMoto;
        static_assert(sizeof(u1007_Mix_VMoto) == sizeof(uint16_t), "The Bits Are Not Aligned");

        QString to_string() const;

        CLteErrorCode::ErrorPkg get_errorString() const;
    };
};
}  // namespace COM

#endif  // CMIXING_H_1709778830

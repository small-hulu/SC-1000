/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       Config_CReagentPlate.h
 * @date       2024-01-03
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      试剂仓的配置信息
 *****************************************************************************/
#ifndef CREAGENTPLATE_H_1704250062
#define CREAGENTPLATE_H_1704250062

#include <QString>

#include "../../config/CLteErrorCode.hpp"

namespace COM {

struct CReagentBin {
    struct Params {
        bool isValid = false;

        int32_t  position = 0;  /// 位置
        int32_t  velocity = 0;  /// 速度
        uint16_t LED_R    = 0;  /// 灯带RGB-R
        uint16_t LED_G    = 0;  /// 灯带RGB-G
        uint16_t LED_B    = 0;  /// 灯带RGB-B
        QString  to_string() const;
    };

    struct StateBits {
        bool isValid = false;

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_run : 1;              // 00-转盘运动标志
                uint8_t : 1;                      // [占位]
                uint8_t b02_isReachPosition : 1;  // 02-是否达到目标位置
                uint8_t b03_isReachVelocity : 1;  // 03-是否达到目标速度
                uint8_t : 2;                      // [占位]
                uint8_t b06_runningError : 1;     // 06-转盘运动错误
                uint8_t : 1;                      // [占位]
                uint8_t b07_hasLid : 1;           // 07-试剂盘保温盖
                uint8_t : 7;                      // [占位]
            };
        } u1002;
        static_assert(sizeof(u1002) == sizeof(uint16_t), "The Bits Are Not Aligned");

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_selfCheck : 1;          // 00-自检完成
                uint8_t b01_ErrorAssembly : 1;      // 01-部件错误
                uint8_t : 5;                        // [占位]
                uint8_t b07_homing : 1;             // 07-归位完成
                uint8_t : 1;                        // [占位]
                uint8_t b09_ErrorXDrive : 1;        // 09-x驱动
                uint8_t : 2;                        // [占位]
                uint8_t b12_ErrorRunTimeOut : 1;    // 12-动作超时
                uint8_t b13_ErrorXLockedRotor : 1;  // 13-x堵转
                uint8_t : 2;                        // [占位]
            };
        } u1003;
        static_assert(sizeof(u1003) == sizeof(uint16_t), "The Bits Are Not Aligned");

        QString to_string() const;

        CLteErrorCode::ErrorPkg get_errorString() const;
    };
};

}  // namespace COM

#endif  // CREAGENTPLATE_H_1704250062

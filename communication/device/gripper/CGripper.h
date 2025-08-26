/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       Config_CGripper.h
 * @date       2024-03-06
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      夹爪的状态
 *
 *****************************************************************************/
#ifndef CGRIPPER_H_1704336442
#define CGRIPPER_H_1704336442

#include <QString>

#include "../../config/CLteErrorCode.hpp"

namespace COM {

struct CGripper {
    struct Params {
        bool isValid = false;

        int32_t xPosition   = 0;  /// x轴位置
        int32_t yPosition   = 0;  /// y轴位置
        int32_t zPosition   = 0;  /// z轴位置
        int32_t deepGrab    = 0;  /// 夹取深度
        int32_t deepRelease = 0;  /// 释放深度
        int32_t openRange   = 0;  /// 张开范围 （只在调试模式下运行）
        int32_t rangeMin    = 0;  /// 张开零点
        int32_t rangeMax    = 0;  /// 张开极限

        QString to_string() const;
    };

    struct StateBits {
        bool isValid = false;

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_run : 1;            // 00-夹爪运动标志
                uint8_t b01_zAxisIsRuning : 1;  // 01-z轴高位标志
                uint8_t b02_xyAxisReach : 1;    // 02-xy位置到达标志
                uint8_t b03_refuse : 1;         // 03-拒绝执行标志
                uint8_t b04_crash : 1;          // 04-碰撞标志
                uint8_t b05_success : 1;        // 05-动作成功标志
                uint8_t b06_runError : 1;       // 06-夹爪运动错误标志
                uint8_t b07_itemExist : 1;      // 07-夹取物存在标志
                uint8_t b08_modeReady : 1;      // 08-模式准备完成
                uint8_t : 3;                    // [占位]
                uint8_t b12_leisure : 1;        // 12-动作空闲
                uint8_t : 3;                    // [占位]
            };
        } u1002;
        static_assert(sizeof(u1002) == sizeof(uint16_t), "The Bits Are Not Aligned");

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_selfCheck : 1;          // 00-自检完成标志
                uint8_t b01_ErrorAssembly : 1;      // 01-部件错误
                uint8_t : 5;                        // [占位]
                uint8_t b07_ErrorGDrive : 1;        // 07-g驱动
                uint8_t b08_ErrorGLockedRotor : 1;  // 08-g堵转
                uint8_t b09_ErrorXDrive : 1;        // 09-x驱动
                uint8_t b10_ErrorYDrive : 1;        // 10-y驱动
                uint8_t b11_ErrorZDrive : 1;        // 11-z驱动
                uint8_t b12_ErrorRunTimeOut : 1;    // 12-动作超时
                uint8_t b13_ErrorXLockedRotor : 1;  // 13-x堵转
                uint8_t b14_ErrorYLockedRotor : 1;  // 14-y堵转
                uint8_t b15_ErrorZLockedRotor : 1;  // 15-z堵转
                uint8_t : 0;                        // [占位]
            };
        } u1003;
        static_assert(sizeof(u1003) == sizeof(uint16_t), "The Bits Are Not Aligned");

        QString to_string() const;

        CLteErrorCode::ErrorPkg get_errorString() const;
    };
};

}  // namespace COM

#endif  // CGRIPPER_H_1704336442

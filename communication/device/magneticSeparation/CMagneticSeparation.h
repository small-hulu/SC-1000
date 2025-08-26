/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       Config_CMagneticSeparation.h
 * @date       2024-01-09
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      磁分离模块状态
 *
 *****************************************************************************/
#ifndef MAGNETICSEPARATION_H_1704791434
#define MAGNETICSEPARATION_H_1704791434

#include <QString>

#include "../../config/CLteErrorCode.hpp"

namespace COM {

struct CMagneticSeparation {
    struct Params {
        bool isValid = false;

        int32_t position           = 0;  /// 托盘位置
        int32_t washCapacity       = 0;  /// 清洗液加注的容量
        int32_t aSubstrateCapacity = 0;  /// 底物A加注的容量
        int32_t drainDeep          = 0;  /// 排液深度
        int32_t mixHeight          = 0;  /// 混匀高度
        int32_t zAxisHeight        = 0;  /// z轴高度

        QString to_string() const;
    };

    struct StateBits {
        bool isValid = false;

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_run : 1;       // 00-托盘运动标志
                uint8_t b01_zAxisRun : 1;  // 01-z轴位置标志
                uint8_t : 1;               // [占位]
                uint8_t b03_success : 1;   // 03-执行成功标志
                uint8_t b04_refuse : 1;    // 04-拒绝执行标志
                uint8_t b05_crash : 1;     // 05-碰撞标志
                uint8_t : 2;               // [占位]

                uint8_t b08_modeReady : 1;  // 08-模式准备完成
                uint8_t : 3;                // [占位]
                uint8_t b12_leisure : 1;    // 12-动作空闲
                uint8_t : 3;                // [占位]
            };
        } u1002;
        static_assert(sizeof(u1002) == sizeof(uint16_t), "The Bits Are Not Aligned");

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_selfCheck : 1;          // 00-自检完成标志
                uint8_t b01_ErrorAssembly : 1;      // 01-部件错误
                uint8_t : 5;                        // [占位]
                uint8_t b07_ErrorSDrive : 1;        // 07-S驱动
                uint8_t b08_ErrorWDrive : 1;        // 08-W驱动
                uint8_t b09_ErrorZDrive : 1;        // 09-Z驱动
                uint8_t b10_ErrorRDrive : 1;        // 10-R驱动
                uint8_t b11_ErrorPDrive : 1;        // 11-P驱动
                uint8_t b12_ErrorRunTimeOut : 1;    // 12-动作超时
                uint8_t b13_ErrorZLockedRotor : 1;  // 13-Z堵转
                uint8_t b14_ErrorRLockedRotor : 1;  // 14-R堵转
                uint8_t b15_ErrorPLockedRotor : 1;  // 15-P堵转
            };
        } u1003;
        static_assert(sizeof(u1003) == sizeof(uint16_t), "The Bits Are Not Aligned");

        QString to_string() const;

        CLteErrorCode::ErrorPkg get_errorString() const;
    };
};
}  // namespace COM

#endif  // MAGNETICSEPARATION_H_1704791434

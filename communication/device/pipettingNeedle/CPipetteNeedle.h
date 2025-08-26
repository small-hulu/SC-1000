/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       Config_CPipettingNeedle.h
 * @date       2024-03-06
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      移液针的控制
 *
 *****************************************************************************/
#ifndef CPIPETTINGNEEDLE_H_1709692657
#define CPIPETTINGNEEDLE_H_1709692657

#include <QString>

#include "../../config/CLteErrorCode.hpp"

namespace COM {
struct CPipetteNeedle {
    struct Params {
        bool isValid = false;

        int32_t yPosition         = 0;  /// y轴位置
        int32_t zPosition         = 0;  /// z轴位置
        int32_t zPosition1        = 0;  /// z轴深度的备选位置
        int32_t suctionZPosition  = 0;  /// 吸液时下探的z轴深度
        int32_t drainageZPosition = 0;  /// 排液时下探的z轴深度
        int32_t suctionCapacity   = 0;  /// 吸液的容量
        int32_t washYPosition     = 0;  /// 清洗池Y位置
        int32_t washZPosition     = 0;  /// 清洗池Z位置
        int16_t suctionThreshold  = 0;  /// 吸液门限
        int32_t descentCache      = 0;  /// 下探深度锁存
        int32_t delayWashTime     = 0;  /// 清洗液排空时间

        QString to_string() const;
    };

    struct StateBits {
        bool isValid = false;

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_isrunning : 1;                // 00-采样针运动标志
                uint8_t b01_zAxisIsMax : 1;               // 01-z轴是否在最高位
                uint8_t b02_yAxisReached : 1;             // 02-y轴坐标到达
                uint8_t b03_actionSuccess : 1;            // 03-动作成功
                uint8_t b04_crash : 1;                    // 04-撞针
                uint8_t b05_InsufficientReagent_in : 1;   // 05-试剂不足-进液异常
                uint8_t b06_runError : 1;                 // 06-运动错误标志
                uint8_t b07_fillBottleTimeout : 1;        // 07-缓冲瓶充液超时
                uint8_t b08_modeReady : 1;                // 08-模式准备完成
                uint8_t : 1;                              // [占位]
                uint8_t b10_isLeisure : 1;                // 10-动作空闲
                uint8_t b11_InsufficientReagent_out : 1;  // 11-试剂不足-出液异常
                uint8_t b12_refuse : 1;                   // 12-拒绝执行
                uint8_t b13_NeedleBlockError : 1;         // 13-堵针检测
                uint8_t b14_BufferBottleUpper : 1;        // 缓冲瓶高位传感器
                uint8_t b15_BufferBottleLower : 1;        // 缓冲瓶地位传感器
            };
        } u1002;
        static_assert(sizeof(u1002) == sizeof(uint16_t), "The Bits Are Not Aligned");

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_selfCheck : 1;          // 00-自检完成标志
                uint8_t b01_ErrorAssembly : 1;      // 01-部件错误
                uint8_t : 6;                        // [占位]
                uint8_t : 1;                        // [占位]
                uint8_t b09_ErrorPDrive : 1;        // 09-P驱动
                uint8_t b10_ErrorYDrive : 1;        // 10-Y驱动
                uint8_t b11_ErrorZDrive : 1;        // 11-Z驱动
                uint8_t b12_ErrorRunTimeOut : 1;    // 12-动作超时
                uint8_t : 1;                        // [占位]
                uint8_t b14_ErrorYLockedRotor : 1;  // 14-y堵转
                uint8_t b15_ErrorZLockedRotor : 1;  // 15-z堵转
            };
        } u1003;
        static_assert(sizeof(u1003) == sizeof(uint16_t), "The Bits Are Not Aligned");

        QString to_string() const;

        CLteErrorCode::ErrorPkg get_errorString() const;
    };
};
}  // namespace COM

#endif  // CPIPETTINGNEEDLE_H_1709692657

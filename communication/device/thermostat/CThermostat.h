#ifndef CTHERMOSTAT_H_1710743502
#define CTHERMOSTAT_H_1710743502

#include <QString>

#include "../../config/CLteErrorCode.hpp"

namespace COM {

/**
 * @brief The CThermostat class
 * 加热模块
 * 包含：
 * - 光电组件s
 * - 温裕盘
 * - 试剂仓
 * - 风扇（额外4个小风扇）
 */
struct CThermostat {
    /**
     * 上位机设置参数
     * =============================
     * 只有“温裕盘”是需要设定参数的
     */
    struct Params {
        bool isValid = false;

        /// 光电仓
        bool  photoelectricAble     = false;  /// 光电组件使能
        float photoelectricTemp     = 0;      /// 光电组件温度
        float photoelectricElectric = 0;      /// 光电组件电流

        /// 温裕盘
        bool  incubateAble     = false;  /// 温裕盘使能
        float incubateTemp     = 0;      /// 温裕盘目标温度（设置）
        float incubateElectric = 0;      /// 温裕盘电流

        /// 试剂仓盖
        bool  reagentBinLidAble     = false;  /// 试剂仓盖使能
        float reagentBinLidTemp     = 0;      /// 试剂仓盖温度
        float reagentBinLidElectric = 0;      /// 试剂仓盖电流

        /// 试剂仓
        bool  reagentBinAble             = false;  /// 试剂仓使能
        float reagentBinTemp             = 0;      /// 试剂仓温度
        float reagentBinPeltierFrontTemp = 0;      /// 试剂仓帕尔贴正面温度
        float reagentBinPeltierBackTemp  = 0;      /// 试剂仓帕尔贴背面温度
        float reagentBinPEL1Electric     = 0;      /// 加热电流 PEL1
        float reagentBinPEL2Electric     = 0;      /// 加热电流 PEL2
        float reagentBinOutElectric      = 0;      /// 设备外温度
        /// 试剂仓的风扇
        bool fan0Able                    = false;  /// 风扇1使能
        bool fan1Able                    = false;  /// 风扇2使能
        bool fan2Able                    = false;  /// 风扇3使能
        bool fan3Able                    = false;  /// 风扇4使能

        QString to_string() const;
    };

    struct StateBits {
        bool isValid = false;

        /**
         * 1002
         * 光电组件状态寄存器
         */
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_isError : 1;                // 00-异常标志
                uint8_t b01_isRunning : 1;              // 01-变温标志
                uint8_t b02_isChangeError : 1;          // 02-加热制冷状态异常
                uint8_t b03_isChangeTimeOutWaring : 1;  // 03-变温超时
                uint8_t b04_isChangeTempOutWaring : 1;  // 04-超温警告
                uint8_t : 3;                            // [占位]
                uint8_t : 8;                            // [占位]
            };
        } u1002_photoelectric;
        static_assert(sizeof(u1002_photoelectric) == sizeof(uint16_t), "The Bits Are Not Aligned");

        /**
         * 1007
         * 温裕盘
         */
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_isError : 1;                // 00-异常标志
                uint8_t b01_isRunning : 1;              // 01-变温标志
                uint8_t b02_isChangeError : 1;          // 02-加热制冷状态异常
                uint8_t b03_isChangeTimeOutWaring : 1;  // 03-变温超时
                uint8_t b04_isChangeTempOutWaring : 1;  // 04-超温警告
                uint8_t : 3;                            // [占位]
                uint8_t : 8;                            // [占位]
            };
        } u1007_thermal;
        static_assert(sizeof(u1007_thermal) == sizeof(uint16_t), "The Bits Are Not Aligned");

        /**
         * 1014
         * 试剂仓盖
         */
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_isError : 1;                // 00-异常标志
                uint8_t b01_isRunning : 1;              // 01-变温标志
                uint8_t b02_isChangeError : 1;          // 02-加热制冷状态异常
                uint8_t b03_isChangeTimeOutWaring : 1;  // 03-变温超时
                uint8_t b04_isChangeTempOutWaring : 1;  // 04-超温警告
                uint8_t : 3;                            // [占位]
                uint8_t : 8;                            // [占位]
            };
        } u1014_reagentBinLid;
        static_assert(sizeof(u1014_reagentBinLid) == sizeof(uint16_t), "The Bits Are Not Aligned");

        /**
         * 1020
         * 试剂仓
         */
        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_isError : 1;                // 00-异常标志
                uint8_t b01_isRunning : 1;              // 01-变温标志
                uint8_t b02_isChangeError : 1;          // 02-加热制冷状态异常
                uint8_t b03_isChangeTimeOutWaring : 1;  // 03-变温超时
                uint8_t b04_isChangeTempOutWaring : 1;  // 04-超温警告
                uint8_t : 3;                            // [占位]
                uint8_t : 8;                            // [占位]
            };
        } u1020_reagentBin;
        static_assert(sizeof(u1020_reagentBin) == sizeof(uint16_t), "The Bits Are Not Aligned");

        QString to_string() const;

        CLteErrorCode::ErrorPkg get_errorString() const;
    };
};
}  // namespace COM
#endif  // CTHERMOSTAT_H_1710743502

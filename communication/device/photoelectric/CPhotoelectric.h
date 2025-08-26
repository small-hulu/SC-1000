/** ***************************************************************************
 * Copyright © 2024 luyinfeng
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       Config_CPhotoelectric.h
 * @date       2024-03-08
 * @author     lotus
 * @Email      luyinfeng@smtbio.com
 * @version    0.0.0
 *
 * @brief      光电组件
 * 无参数
 *
 *****************************************************************************/
#ifndef CPHOTOELECTRIC_H_1709794772
#define CPHOTOELECTRIC_H_1709794772

#include <QString>

#include "../../config/CLteErrorCode.hpp"

namespace COM {
struct CPhotoelectric {
    struct Params {
        bool isValid = false;

        int32_t capacity   = 0;  /// 底物B容量
        int32_t positionIn = 0;  /// 检测位
        int32_t position   = 0;  /// debug值

        QString to_string() const;
    };

    struct StateBits {
        bool isValid = false;

        union {
            uint16_t ALL = 0;
            struct {
                uint8_t b00_leisure : 1;            // 00-动作空闲
                uint8_t b01_success : 1;            // 01-动作成功
                uint8_t b02_detectError : 1;        // 02-检测失败
                uint8_t b03_pumpIsEmpty : 1;        // 03-泵空
                uint8_t : 3;                        // [占位]
                uint8_t b07_counterInit : 1;        // 07-计数器初始化正常
                uint8_t b08_ErrorAssembly : 1;      // 08-部件错误
                uint8_t : 3;                        // [占位]
                uint8_t b12_ErrorRunTimeOut : 1;    // 12-动作超时
                uint8_t b13_ErrorSDrive : 1;        // 13-s驱动错误
                uint8_t b14_ErrorXDrive : 1;        // 14-x驱动错误
                uint8_t b15_ErrorXLockedRotor : 1;  // 15-电机堵转
            };
        } u1001;
        static_assert(sizeof(u1001) == sizeof(uint16_t), "The Bits Are Not Aligned");

        QString to_string() const;

        CLteErrorCode::ErrorPkg get_errorString() const;
    };
};
}  // namespace COM

#endif  // CPHOTOELECTRIC_H_1709794772

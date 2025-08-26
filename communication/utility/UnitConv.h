/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       UnitConv.h
 * @date       2024-08-24
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 后期应该要从文件中读系数
 *
 *****************************************************************************/
#ifndef UNITCONV_H_1724487961
#define UNITCONV_H_1724487961

#include "../config/CParams.hpp"

namespace COM {
/**
 * 移液针吸取量
 * =========
 * 微升<->电机参数
 */
inline int microlitre_to_motorStep_PipetteNeedle(int val) {
    return val * MotorRatio;
}
inline int motorStep_to_microlitre_PipetteNeedle(int val) {
    return val / MotorRatio;
}

/**
 * 底物A排放量
 * =========
 * 微升<->电机参数
 */
inline int microlitre_to_motorStep_ASubstrate(int val) {
    return val * MotorRatio;
}
inline int motorStep_to_microlitre_ASubstrate(int val) {
    return val / MotorRatio;
}

/**
 * 底物B排放量
 * =========
 * 微升<->电机参数
 */
inline int microlitre_to_motorStep_BSubstrate(int val) {
    return val * MotorRatio;
}
inline int motorStep_to_microlitre_BSubstrate(int val) {
    return val / MotorRatio;
}

}  // namespace COM

#endif  // UNITCONV_H_1724487961

/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       CDevice_Maintain.h
 * @date       2024-06-12
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 配合维护界面的各个任务
 *
 *****************************************************************************/
#ifndef MAINTAIN_H_1718171213
#define MAINTAIN_H_1718171213

namespace CONTROLLER::CDevice {
struct DMaintain {
    //! 所有部件homing
    static void Homing_allDevice();
    //! 泵homing
    static void Homing_pump();
    //! 清管
    static void Maintain_TubleDisCard();
    //! 磁分离清洗液灌注
    static void Maintain_AbleMagneticSeparationWash();
    //! 底物A灌注
    static void Maintain_AbleASubstrate();
    //! 底物B灌注
    static void Maintain_AbleBSubstrate();
    //! 排空清洗液
    static void Maintain_WashDrain();
    //! 加注清洗液
    static void Maintain_WashFill();
    //! 加注清洗液
    static void Maintain_WashIntensive();
};
}  // namespace CONTROLLER::CDevice
#endif  // MAINTAIN_H_1718171213

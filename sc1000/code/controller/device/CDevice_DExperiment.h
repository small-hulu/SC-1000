/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       CONTROLLER_Experiment.h
 * @date       2024-05-29
 * @version    1.0.0
 *
 * @author     陆寅峰
 *
 * @brief
 * 实验的总控制中枢
 *
 *****************************************************************************/
#ifndef EXPERIMENT_H_1716960544
#define EXPERIMENT_H_1716960544
namespace CONTROLLER::CDevice {

class DExperiment {
public:
    static void Strat();
    static void Pause();
    static void Stop();

private:
    static void analyse_expSample();
    static void analyse_expCalibration();
    static void analyse_expQuality();
};

}  // namespace CONTROLLER::CDevice

#endif  // EXPERIMENT_H_1716960544

/** ***************************************************************************
 * Copyright © 2024 cuber-lotus
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       4plc_solver.h
 * @date       2024-07-02
 * @version    1.0.0
 *
 * @author     cuber-lotus
 * @Email      luyinfeng@smtbio.com
 *
 * @brief
 * 最原始的方式
 * 就是不带校准的，直接使用四参数计算
 *
 *****************************************************************************/
#ifndef solver_4plc_1719904701
#define solver_4plc_1719904701

#include <vector>

#include "algorithm_type.h"

struct FourPLCWithCal {
    struct Point {
        double rlu           = 0;  // 发光值
        double concentration = 0;  // 浓度
    };
    FourPLC fourplc;
    FourPLC fitting_fourplc;
    /// 请保证基于发光值排序
    std::vector<Point> pointList;
};
FourPLC Adjust_AD_FromPointList(const std::vector<FourPLCWithCal::Point>& pointList, const FourPLC& oldPlc);
/**
 * 上下边界重新拟合
 */
FourPLC Adjust_AD_FromTwoCalibrators(double x0, double y0,double x1, double y1,double B, double C);

/**
 * 基于4plc
 * 发光值 -> 浓度
 */
RetValue alg4plc_rlu_to_concentration(FourPLC fourplc, double input_rlu);
/**
 * 基于4plc
 * 浓度 -> 发光值
 */
RetValue alg4plc_concentration_to_rlu(FourPLC fourplc, double concentration);
/**
 * 基于4plc（基于校准）
 * 发光值 -> 浓度
 */
RetValue alg4plc_rlu_to_concentration_withCal(FourPLCWithCal par, double input_rlu);

#endif  // solver_4plc_1719904701

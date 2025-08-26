#include "algorithm_4plc.h"
#include <math.h>
#include <iostream>

RetValue alg4plc_concentration_to_rlu(FourPLC fourplc, double concentration) {
    const double a = fourplc.A;
    const double b = fourplc.B;
    const double c = fourplc.C;
    const double d = fourplc.D;

    RetValue ret;

    if (concentration < 0) {
        ret.is_valid = false;
        return ret;
    }

    double x = concentration;
    double out = (a - d) / (1.0 + pow(x / c, b)) + d;

    if (isnan(out)) {
        ret.is_valid = false;
        return ret;
    }

    ret.is_valid = true;
    ret.value    = out;
    return ret;
}

RetValue alg4plc_rlu_to_concentration(FourPLC fourplc, double input_rlu) {
    const double a = fourplc.A;
    const double b = fourplc.B;
    const double c = fourplc.C;
    const double d = fourplc.D;

    RetValue ret;

    double out = c * pow((((a - d) / (input_rlu - d)) - 1), (1 / b));

    if (isnan(out)) {
        ret.is_valid = false;
        return ret;
    }

    if (out < 0) {
        out = 0;
    }

    ret.is_valid = true;
    ret.value    = out;
    return ret;
}
RetValue alg4plc_rlu_to_concentration_withCal(FourPLCWithCal par, double input_rlu) {
    RetValue ret;
    auto&& pointList = par.pointList;
    if (pointList.size() < 2) {
        ret.is_valid = false;
        return ret;
    }
    int i;
    for (i = 0; i < (int)pointList.size(); i++) {
        if (input_rlu < pointList[i].rlu) {
            break;
        }
    }

    int front_point;
    int back_point;
    if (i == 0) {
        front_point = 0;
        back_point = 1;
    } else if (i == (int)pointList.size()) {
        front_point = i - 2;
        back_point = i - 1;
    } else {
        front_point = i - 1;
        back_point = i;
    }

    const double front_point_rlu = pointList[front_point].rlu;
    const double front_point_con = pointList[front_point].concentration;

    const double back_point_rlu = pointList[back_point].rlu;
    const double back_point_con = pointList[back_point].concentration;

    // get front point from main curve
    ret = alg4plc_rlu_to_concentration(par.fourplc, front_point_rlu);
    if (!ret.is_valid) {
        qDebug() << "[ERROR] front_point_rlu 在 4PLC 主曲线计算失败: " << front_point_rlu;
        return ret;
    }
    double front_point_con_main_curve = ret.value;
    qDebug() << "front_point_main_curve_concentration:" << front_point_con_main_curve;

    // get back point from main curve
    ret = alg4plc_rlu_to_concentration(par.fourplc, back_point_rlu);
    if (!ret.is_valid) {
        qDebug() << "[ERROR] back_point_rlu 在 4PLC 主曲线计算失败: " << back_point_rlu;
        return ret;
    }
    double back_point_con_main_curve = ret.value;
    qDebug() << "back_point_main_curve_concentration:" << back_point_con_main_curve;

    // get delta
    double con_delta = ((back_point_con - back_point_con_main_curve) -
                        (front_point_con - front_point_con_main_curve)) /
            (back_point_rlu - front_point_rlu);

    // 容错限幅 delta 固定阈值 ±0.00001
    if (con_delta > 0.00001) {
        con_delta = 0.000001;
    } else if (con_delta < -0.00001) {
        con_delta = -0.000001;
    }

    // get offset
    double con_offset = front_point_con - front_point_con_main_curve;

    qDebug() << "delta(限幅后):" << con_delta << "offset:" << con_offset;

    // get target rlu concentration in main curve
    ret = alg4plc_rlu_to_concentration(par.fourplc, input_rlu);
    if (!ret.is_valid) {
        qDebug() << "[ERROR] input_rlu 在 4PLC 主曲线计算失败: " << input_rlu;
        return ret;
    }
    double target_con_main = ret.value;
    qDebug() << "target_concentration_main_curve:" << target_con_main;

    // calculate final value
    double target_con = target_con_main + con_offset + (input_rlu - front_point_rlu) * con_delta;
    qDebug() << "最终浓度:" << target_con;

    ret.value = target_con;
    ret.is_valid = true;
    return ret;
}

FourPLC Adjust_AD_FromPointList(const std::vector<FourPLCWithCal::Point>& pointList, const FourPLC& oldPlc)
{
    if (pointList.size() < 2) {
        throw std::runtime_error("点数不足，无法拟合");
    }
    double x0 = pointList.front().concentration;
    double y0 = pointList.front().rlu;
    double x1 = pointList.back().concentration;
    double y1 = pointList.back().rlu;

    double B = oldPlc.B;
    double C = oldPlc.C;

    return Adjust_AD_FromTwoCalibrators(x0, y0, x1, y1, B, C);
}

FourPLC Adjust_AD_FromTwoCalibrators(double x0, double y0, double x1, double y1, double B, double C)
{
    // Step 1: Compute t0, t1
    double t0 = std::pow(x0 / C, B);
    double t1 = std::pow(x1 / C, B);

    double k0 = 1.0 / (1.0 + t0);
    double k1 = 1.0 / (1.0 + t1);

    // Step 2: Compute denominator
    double denominator = k0 * (1.0 - k1) - k1 * (1.0 - k0);

    if (std::abs(denominator) < 1e-12) {
        throw std::runtime_error("Denominator too small, calibration points too close or ill-conditioned.");
    }

    // Step 3: Solve for A' and D'
    double A = (y0 * (1.0 - k1) - y1 * (1.0 - k0)) / denominator;
    double D = (y1 * k0 - y0 * k1) / denominator;

    // Step 4: Return new parameters
    return FourPLC{A, B, C, D};
}

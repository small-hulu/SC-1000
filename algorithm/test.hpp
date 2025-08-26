#ifndef TEST_HPP
#define TEST_HPP

#include <QDebug>
#include <QVector>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#include "algorithm/algorithm.hpp"

inline void test_0() {
    // 计算给定 y 值的 X
    auto calculate_x = [](double y, double A, double B, double C, double D) -> double {
        if (y == D) {
            return 0;  // 避免除以零
        }
        double term = fabs((A - D) / (y - D) - 1);
        if (term < 0) {
            return -1;  // 无解
        }
        return C * std::pow(term, 1.0 / B);
    };

    // 添加数据点
    QVector<QPointF> dataPoints;
    dataPoints.append(QPointF(0, 1976));
    dataPoints.append(QPointF(0.0253776216144611, 12422.5));
    // dataPoints.append(QPointF(0.036240390891644, 14264));
    dataPoints.append(QPointF(0.0510373900182979, 19112));
    dataPoints.append(QPointF(0.529459694924067, 213079.5));
    // dataPoints.append(QPointF(10.1177548502096, 4651123));
    dataPoints.append(QPointF(25.1496669388851, 12086011));
    dataPoints.append(QPointF(50, 23592448.5));

    FitAlgo fitAlgo;
    fitAlgo.add_Data_Point(dataPoints);

    // 进行四参数拟合（Rodbard 拟合）
    double a, b, c, d;
    /// 该函数永远为 true
    if (fitAlgo.GetRodbard(a, b, c, d)) {
        qDebug() << "Rodbard 拟合结果:";
        qDebug() << "a:" << a;
        qDebug() << "b:" << b;
        qDebug() << "c:" << c;
        qDebug() << "d:" << d;
    } else {
        qDebug() << "Rodbard 拟合失败";
    }

    QVector<double> y_values = {
        2218,    1668,    1570,    2612,    3237,    3673,    3784,    2756,    3775,    3652,
        1661830, 1157988, 6214456, 26626,   3383223, 1281139, 1432186, 1981528, 1223823, 587650,
        3697,    1512,    4917,    3745,    4076,    11932,   6258,    1499,    3376,    5019,
        2369479, 1691322, 7052016, 22864,   300285,  439762,  1424256, 55238,   12243,   3318870,
        4983,    1957,    1816,    2068,    7849,    4824,    4089,    5175,    2078,    7399,
        376419,  4762636, 2433010, 4479526, 193381,  66834,   914971,  419439,  5281789, 183136,
        2059,    2758,    4812,    3904,    4747,    7873,    3604,    6154,    2449,    5122,
        25592,   17025,   1761592, 3911,    101160,  151686,  197165,  11056,   11135,   224893,
        251515,  22376,   8231,    1525194, 1068347, 4260988, 514917,  26037,   12096,   23675,
        560069,  107463,  9556,    834099,  30268,   104307,  10574,   1580763, 2589713, 28042,
        1093777, 106114,  217386};

    y_values.clear();
    y_values.push_back(1661830);
    for (double y : y_values) {
        double x = calculate_x(y, a, b, c, d);
        qDebug() << QString::asprintf("y=%lf x=%lf", y, x);
    }
}

inline void test_1(FourPLC plc) {
    std::vector<double> arr = {
        2218,    1668,    1570,    2612,    3237,    3673,    3784,    2756,    3775,    3652,
        1661830, 1157988, 6214456, 26626,   3383223, 1281139, 1432186, 1981528, 1223823, 587650,
        3697,    1512,    4917,    3745,    4076,    11932,   6258,    1499,    3376,    5019,
        2369479, 1691322, 7052016, 22864,   300285,  439762,  1424256, 55238,   12243,   3318870,
        4983,    1957,    1816,    2068,    7849,    4824,    4089,    5175,    2078,    7399,
        376419,  4762636, 2433010, 4479526, 193381,  66834,   914971,  419439,  5281789, 183136,
        2059,    2758,    4812,    3904,    4747,    7873,    3604,    6154,    2449,    5122,
        25592,   17025,   1761592, 3911,    101160,  151686,  197165,  11056,   11135,   224893,
        251515,  22376,   8231,    1525194, 1068347, 4260988, 514917,  26037,   12096,   23675,
        560069,  107463,  9556,    834099,  30268,   104307,  10574,   1580763, 2589713, 28042,
        1093777, 106114,  217386};

    arr.push_back(14264);
    arr.push_back(13668);
    arr.push_back(4651123);
    arr.push_back(4539455);

    FourPLCWithCal params;
    params.fourplc = plc;

    FourPLCWithCal::Point point;
    point.rlu           = 1.0 * 14264 + 13668 / 2;
    point.concentration = 0.0362;
    params.pointList.push_back(point);
    arr.push_back(point.rlu);

    point.rlu           = 1.0 * 4651123 + 4539455 / 2;
    point.concentration = 10.1178;
    params.pointList.push_back(point);
    arr.push_back(point.rlu);

    for (auto x : arr) {
        auto ret = alg4plc_rlu_to_concentration_withCal(params, x);
        printf("[%s] %10d \t %.6lf\n", ret.is_valid ? "True " : "False", (int)x, ret.value);
    }

    for (auto x : arr) {
        auto ret = alg4plc_rlu_to_concentration_withCal(params, x);
        printf("%.6lf\n", ret.value);
    }
}

inline void test_2() {
    FourPLC plc;
    plc.A = 1345.6345083828076;
    plc.B = 0.9897827748058928;
    plc.C = 25677.769432822686;
    plc.D = 5280814395.4145975;

    FourPLCWithCal params;
    params.fourplc = plc;

    FourPLCWithCal::Point point;
    point.rlu           = 44626;
    point.concentration = 0.427;
    params.pointList.push_back(point);
    point.rlu           = 991715.6;
    point.concentration = 10.4184;
    params.pointList.push_back(point);

    /**
     *  201790, 217309, 348965, 245572, 196671
     *  [True ]     201790       2.075427
     *  [True ]     217309       2.238636
     *  [True ]     348965       3.625009
     *  [True ]     245572       2.535999
     *  [True ]     196671       2.021604
     */
    /**
     *  998791,990442,924714,921405,1123226, 196671
     * [True ]     998791       10.493329
     * [True ]     990442       10.404913
     * [True ]     924714       9.708973
     * [True ]     921405       9.673943
     * [True ]    1123226       11.811482
     */
    for (auto x : {201790, 217309, 348965, 245572, 196671}) {
        auto ret = alg4plc_rlu_to_concentration_withCal(params, x);
        printf("[%s] %10d \t %.6lf\n", ret.is_valid ? "True " : "False", (int)x, ret.value);
    }
}

#endif  // TEST_HPP

#ifndef ALGORITHM_TYPE_H_1719905021
#define ALGORITHM_TYPE_H_1719905021

#include <stdint.h>

#include <string>

/**
 * 返回值
 * 目的就是为了获得浓度值value
 */
struct RetValue {
    bool   is_valid = false;
    double value    = 0;
};

/**
 * 4plc的四个参数
 */
struct FourPLC {
    double A = 0;
    double B = 0;
    double C = 0;
    double D = 0;

    std::string to_string() const {
        std::string s;
        s += "A=" + std::to_string(A) + "\n";
        s += "B=" + std::to_string(B) + "\n";
        s += "C=" + std::to_string(C) + "\n";
        s += "D=" + std::to_string(D) + "\n";
        return s;
    }
};

#endif  // ALGORITHM_TYPE_H_1719905021

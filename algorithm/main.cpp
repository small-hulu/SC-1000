#include <QDebug>
#include <cstdio>
#include <iostream>
#include <vector>

#include "algorithm/algorithm.hpp"
#include "test.hpp"

inline void test() {
    qDebug() << __PRETTY_FUNCTION__;
}

int main() {
    qDebug() << __TIME__;
    test();

    test_0();
}

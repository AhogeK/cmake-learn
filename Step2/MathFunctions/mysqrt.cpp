//
// Created by ahogek on 10/13/21.
//
#include <iostream>

// 一个简单的平方根操作计算
double mysqrt(double x) {
    if (x <= 0) {
        return 0;
    }

    double result = x;

    // 进行十次遍历
    for (int i = 0; i < 10; ++i) {
        if (result <= 0) {
            result = 0.1;
        }
        double delta = x - (result * result);
        result = result + 0.5 * delta / result;
        std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
    }
    return result;
}
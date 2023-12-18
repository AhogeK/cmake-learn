//
// Created by ahogek on 10/13/21.
//
#include <iostream>
#include <sstream>

std::string format_double(double value, int precision) {
    std::ostringstream out;
    out.precision(precision);
    out << value;
    std::string result = out.str();

    // 移除多余的0
    while (result.find('.') != std::string::npos && (result.back() == '0' || result.back() == '.')) {
        result.pop_back();
    }

    return result;
}

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
        result = 0.5 * (result + x / result);
        std::cout << "Computing sqrt of " << x << " to be " << format_double(result, 16) << std::endl;
        if (result * result == x) return result;
    }
    return result;
}
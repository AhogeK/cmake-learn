#include <iostream>
#include <string>

#include "TutorialConfig.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // 输出版本号
        std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
                  << Tutorial_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " [number]" << std::endl;
        return 1;
    }

    // 将输入的值转换为double
    const double inputValue = std::stod(argv[1]);
    std::cout << "input value: " << inputValue << std::endl;
    return 0;
}

# cmake-learn Cmake 官方指南学习练习
> 本项目是个人学习项目

## Step 1

Step1/CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the excutable
add_executable(Tutorial tutorial.cxx)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
	                   "${PROJECT_BINARY_DIR}"
			   )
```

> cmake 配置文件

Step1/TutorialConfig.h.in

```c
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```

> cmake configure_file 指令 用于在编译时自动修改@@内容

Step1/tutorial.cxx

```c++
// A simple program that computes the square root of a number

#include <cmath>
#include <iostream>
#include <string>

#include "TutorialConfig.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		// report version
		std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
			<< Tutorial_VERSION_MINOR << std::endl;
		std::cout << "Usage: " << argv[0] << " number" << std::endl;
		return 1;
	}

	// convert input to double
	const double inputValue = std::stod(argv[1]);
    std::cout << "input value: " << inputValue << std::endl;
    return 0;
}
```

> 测试主程序

### 测试
```shell
mkdir build
cd build
cmake ..
cmake --build .
# 成功演示
./Tutorial 20
# 失败演示
./Tutorial
```
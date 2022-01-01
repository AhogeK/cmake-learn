# cmake-learn Cmake 官方指南学习练习
> 本项目是个人学习项目

## 目录
* [Step 1](#step-1)
* [Step 2](#step-2)

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
cd Step1
mkdir build
cd build
cmake ..
cmake --build .
# 成功演示
./Tutorial 20
# 失败演示
./Tutorial
```

[返回目录](#目录)

---

## Step 2

Step2/CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# for larger projects this is a common occurrence. The first step is to add an option to the top-level CMakeLists.txt file
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# conditionally execute a group of commands
if (USE_MYMATH)

    # add the MathFunctions library
    add_subdirectory(MathFunctions)

    # The list subcommands APPEND, INSERT, FILTER, PREPEND, POP_BACK, POP_FRONT, REMOVE_AT, REMOVE_ITEM,
    #   REMOVE_DUPLICATES, REVERSE and SORT may create new values for the list within the current CMake variable scope.
    #   Similar to the set() command, the LIST command creates new variable values in the current scope, even if the
    #   list itself is actually defined in a parent scope.
    list(APPEND EXTRA_LIBS MathFunctions)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif ()

# add the MathFunctions library
# add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)

# EXTRA_LIBS come from 'list(APPEND EXTRA_LIBS MathFunctions)'
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# do not use conditionally
# target_include_libraries(Tutorial PUBLIC MathFunctions)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        # "${PROJECT_SOURCE_DIR}/MathFunctions"
        ${EXTRA_INCLUDES}
        )
```
> 主要添加了一个是否使用自己定义的方法的一个 OPTION

Step2/TutorialConfig.h.in

```c
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
#cmakedefine USE_MYMATH
```
> 添加上述option的定义

Step2/tutorial.cxx
```c++
// A simple program that computes the square root of a number

#include <cmath>
#include <iostream>
#include <string>

#include <TutorialConfig.h>

#ifdef USE_MYMATH
#include <MathFunctions.h>
#endif

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        // report version
        cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
             << Tutorial_VERSION_MINOR << endl;
        cout << "Usage: " << argv[0] << " number" << endl;
        return 1;
    }

    // convert input to double
    const double inputValue = std::stod(argv[1]);
#ifdef USE_MYMATH
    cout << "use my math" << endl;
    const double outputValue = mysqrt(inputValue);
#else
    cout << "use c math" << endl;
    const double outputValue = sqrt(inputValue);
#endif
    cout << "The square root of " << inputValue << " is " << outputValue
         << endl;
    return 0;
}
```
> 使用定义的 define 在有和没有的不同情况下执行不同的代码

Step2/MathFunctions/CMakeLists.txt
```cmake
# add the following one line CMakeLists.txt file to the MathFunctions directory
add_library(MathFunctions mysqrt.cpp)
```
> 自己定义的function库的cmake

Step2/MathFunctions/MathFunctions.h
```c
//
// Created by ahogek on 10/13/21.
//

#ifndef CMAKE_LEARN_MATHFUNCTIONS_H
#define CMAKE_LEARN_MATHFUNCTIONS_H

double mysqrt(double x);

#endif //CMAKE_LEARN_MATHFUNCTIONS_H
```
> 与库名相同的头文件可代表 library mysqrt.cpp文件的头文件来定义方法

Step2/MathFunctions/mysqrt.cpp
```c++
//
// Created by ahogek on 10/13/21.
//
#include <iostream>

// a hack square root calculation using simple operations
double mysqrt(double x) {
    if (x <= 0) {
        return 0;
    }

    double result = x;

    // do ten iterations
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
```
> 自定义库的实际实现

### 测试

```shell
cd Step2
mkdir build
cd build
cmake ..
cmake --build .
./Tutorial 4
# 相当与 cmake 配置文件中 option 选项里的 ON 改 OFF
cmake .. -DUSE_MYMATH=OFF
cmake --build .
./Tutorial 4
```

[返回目录](#目录)

---
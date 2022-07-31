# cmake-learn Cmake 官方指南学习练习
> 本项目是个人学习项目

## 目录
* [Step 1](#step-1)
* [Step 2](#step-2)
* [Step 3](#step-3)
* [Step 4](#step-4)

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
# https://cmake.org/cmake/help/latest/command/configure_file.html
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the executable
# https://cmake.org/cmake/help/latest/command/add_executable.html?highlight=add_executable
add_executable(Tutorial tutorial.cxx)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
# https://cmake.org/cmake/help/latest/command/target_include_directories.html?highlight=target_include_directories
# https://cmake.org/cmake/help/latest/variable/PROJECT_BINARY_DIR.html?highlight=project_binary_dir
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        )
```

> cmake 配置文件

Step1/TutorialConfig.h.in

```c
/*
 * the configured options and settings for Tutorial
 * https://cmake.org/cmake/help/latest/variable/PROJECT-NAME_VERSION_MAJOR.html
 * https://cmake.org/cmake/help/latest/variable/PROJECT-NAME_VERSION_MINOR.html
 */
#define
Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define
Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```

> cmake configure_file 指令 用于在编译时自动修改@@内容

Step1/tutorial.cxx

```c++
// A simple program that computes the square root of a number

#include <iostream>
#include <string>

#include "TutorialConfig.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		// report version
		std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
			<< Tutorial_VERSION_MINOR << std::endl;
		std::cout << "Usage: " << argv[0] << " [number]" << std::endl;
		return 1;
	}

	// convert input to double
	const double inputValue = std::stod(argv[1]);
    std::cout << "input value: " << inputValue << std::endl;
    return 0;
}
```
[std:stod documentation](https://cplusplus.com/reference/string/stod/)

> 测试主程序

### 测试
```shell
cd Step1
mkdir build
cd build
cmake ..
# cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja  -G Ninja -S .. -B .
cmake --build .
# cmake --build . --target Tutorial -j 6
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
# https://cmake.org/cmake/help/latest/command/option.html
option(USE_MY_MATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# conditionally execute a group of commands
if (USE_MY_MATH)

    # add the MathFunctions library
    add_subdirectory(MathFunctions)

    # The list subcommands APPEND, INSERT, FILTER, PREPEND, POP_BACK, POP_FRONT, REMOVE_AT, REMOVE_ITEM,
    #   REMOVE_DUPLICATES, REVERSE and SORT may create new values for the list within the current CMake variable scope.
    #   Similar to the set() command, the LIST command creates new variable values in the current scope, even if the
    #   list itself is actually defined in a parent scope.
    # https://cmake.org/cmake/help/latest/command/list.html
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
#cmakedefine USE_MY_MATH
```
> 添加上述option的定义

Step2/tutorial.cxx
```c++
// A simple program that computes the square root of a number

#include <cmath>
#include <iostream>
#include <string>

#include <TutorialConfig.h>

#ifdef USE_MY_MATH
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
#ifdef USE_MY_MATH
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
cmake .. -DUSE_MY_MATH=OFF
cmake --build .
./Tutorial 4
```

[返回目录](#目录)

---

## Step 3

Step3/CmakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# for larger projects this is a common occurrence. The first step is to add an option to the top-level CMakeLists.txt file
option(USE_MY_MATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# conditionally execute a group of commands
if (USE_MY_MATH)

    # add the MathFunctions library
    add_subdirectory(MathFunctions)

    # The list subcommands APPEND, INSERT, FILTER, PREPEND, POP_BACK, POP_FRONT, REMOVE_AT, REMOVE_ITEM,
    #   REMOVE_DUPLICATES, REVERSE and SORT may create new values for the list within the current CMake variable scope.
    #   Similar to the set() command, the LIST command creates new variable values in the current scope, even if the
    #   list itself is actually defined in a parent scope.
    list(APPEND EXTRA_LIBS MathFunctions)

    # if we use target_include_directories on MathFunctions don't need this
    # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif ()

# add the MathFunctions library
# add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cpp)

# EXTRA_LIBS come from 'list(APPEND EXTRA_LIBS MathFunctions)'
target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# do not use conditionally
# target_include_libraries(Tutorial PUBLIC MathFunctions)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        # "${PROJECT_SOURCE_DIR}/MathFunctions"
        # id.
        # ${EXTRA_INCLUDES}
        )
```

> 取消了Step2中的 "EXTRA_INCLUDES", 该操作直接在 MathFunctions 的 lib Cmake 中进行

Step3/tutorial.cxx Step3/TutorialConfig.h.in

**以上两个文件与 Step2 无异**

Step3/MathFunctions/CMakeLists.txt

```cmake
# add the following one line CMakeLists.txt file to the MathFunctions directory
add_library(MathFunctions mysqrt.cpp)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
```

> 添加了 target_include_directories 作用与 Step2 中主体中的 target_include_directories 相同

**另外 MathFunctions.h 与 mysqrt.cpp 文件不变**

### 测试

```shell
cd Step3
mkdir build
cd build
cmake ..
cmake --build .
./Tutorial 4
```

[返回目录](#目录)

---

## Step 4

Step4/CmakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial VERSION 1.0)

# specify the c++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# for larger projects this is a common occurrence. The first step is to add an option to the top-level CMakeLists.txt file
option(USE_MY_MATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)

# conditionally execute a group of commands
if (USE_MY_MATH)

    # add the MathFunctions library
    add_subdirectory(MathFunctions)

    # The list subcommands APPEND, INSERT, FILTER, PREPEND, POP_BACK, POP_FRONT, REMOVE_AT, REMOVE_ITEM,
    #   REMOVE_DUPLICATES, REVERSE and SORT may create new values for the list within the current CMake variable scope.
    #   Similar to the set() command, the LIST command creates new variable values in the current scope, even if the
    #   list itself is actually defined in a parent scope.
    list(APPEND EXTRA_LIBS MathFunctions)

    # if we use target_include_directories on MathFunctions don't need this
    # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
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
        # id.
        # ${EXTRA_INCLUDES}
        )

# add the install targets
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
        DESTINATION include
        )

enable_testing()

# does the application run
add_test(NAME Runs COMMAND Tutorial 25)

# does the usage message work?
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
        PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
        )

# define a function to simplify adding tests
function(do_test target arg result)
    add_test(NAME Comp${arg} COMMAND ${target} ${arg})
    set_tests_properties(Comp${arg}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result}
            )
endfunction()

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is (-nan|nan|0)")
do_test(Tutorial 0.0001 "0.0001 is 0.01")
```

> 这一节主要添加了测试，这一节用到了很多测试相关的 CMake 指令。
> 详细可以查阅 | [install](https://cmake.org/cmake/help/latest/command/install.html) | 
> [enable_testing](https://cmake.org/cmake/help/latest/command/enable_testing.html?highlight=enable_testing) | 
> [add_test](https://cmake.org/cmake/help/latest/command/add_test.html#command:add_test) | 
> [set_tests_properties](https://cmake.org/cmake/help/latest/command/set_tests_properties.html?highlight=set_tests_properties) | 
> [PASS_REGULAR_EXPRESSION](https://cmake.org/cmake/help/latest/prop_test/PASS_REGULAR_EXPRESSION.html?highlight=pass_regular_expression) | 
> [function](https://cmake.org/cmake/help/latest/command/function.html?highlight=function) |

Step4/MathFunctions/CMakeList.txt

```cmake
# add the following one line CMakeLists.txt file to the MathFunctions directory
add_library(MathFunctions mysqrt.cpp)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# install rules
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
```

> MathFunctions 中的 Cmake 同样添加测试相关 需要 install 两个规则

**Step4 总共修改了两个 Cmake 文件用于单元测试的内容**

### 测试

```shell
cd Step4
mkdir build
cd build
cmake ..
# prefix自定义地址，编译测试用文件 默认会在 /usr/local/bin 中去构建 需要权限不建议
cmake --instal . --prefix "/home/ahogek/Documents/learn-workspace/cmake-learn/Step4/build"
ctest
```

**本节着重理解单元测试的入门写法，不算太难**

[返回目录](#目录)

---

# cmake-learn Cmake 官方指南学习练习

> 本项目是个人学习项目

## 目录

* [Step 1](#step-1)
* [Step 2](#step-2)
* [Step 3](#step-3)
* [Step 4](#step-4)

## Step 1

> cmake 配置文件

Step1/CMakeLists.txt

```cmake
# cmake 的最低要求版本号
cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(Tutorial VERSION 1.0)

# 指定C++标准版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 配置cmake头文件，用于设置一些头属性变量
# https://cmake.org/cmake/help/latest/command/configure_file.html
configure_file(TutorialConfig.h.in TutorialConfig.h)

# 添加可执行文件
# https://cmake.org/cmake/help/latest/command/add_executable.html?highlight=add_executable
add_executable(Tutorial tutorial.cxx)

# 添加依赖目录，用于查询例如上面configure_file生成的头文件
# https://cmake.org/cmake/help/latest/command/target_include_directories.html?highlight=target_include_directories
# https://cmake.org/cmake/help/latest/variable/PROJECT_BINARY_DIR.html?highlight=project_binary_dir
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
)
```

> cmake configure_file 指令 用于在编译时自动修改@@内容

`configure_file(TutorialConfig.h.in TutorialConfig.h)` 是 CMake 的一个命令，它的作用是将输入文件 TutorialConfig.h.in
复制到输出文件 TutorialConfig.h，并在复制过程中替换掉输入文件中的一些变量。 在你的 TutorialConfig.h.in
文件中，有两个变量 `@Tutorial_VERSION_MAJOR@ 和 ${Tutorial_VERSION_MINOR}`。这两个变量在 configure_file 命令执行时会被替换为在
CMakeLists.txt 文件中定义的对应值。 在你的 CMakeLists.txt 文件中，你定义了项目的版本号 `project(Tutorial VERSION 1.0)`
，这将设置 Tutorial_VERSION_MAJOR 和 Tutorial_VERSION_MINOR 的值。然后，configure_file 命令会将这些值替换到
TutorialConfig.h.in 文件中的对应位置，生成 TutorialConfig.h 文件。
所以，`configure_file(TutorialConfig.h.in TutorialConfig.h)` 命令的作用是生成一个包含项目版本号的头文件 TutorialConfig.h。

在 CMake 中，**`target_include_directories`**
是一个非常重要的命令，用于为目标（例如库或可执行文件）指定包含（include）目录。在编译这个目标时，这些目录会被添加到编译器的包含路径中。这对于解决头文件的依赖关系非常有用。

### **命令结构**

```cmake
target_include_directories(<target> <PRIVATE|PUBLIC|INTERFACE> <dir1> <dir2> ...)
```

- **`<target>`**: 目标的名称，即你要为其指定包含目录的目标（库或可执行文件）。
- **`<PRIVATE|PUBLIC|INTERFACE>`**: 指定包含目录的作用范围。
    - **`PRIVATE`**: 目录仅对该目标有效，不会传递给依赖该目标的其他目标。
    - **`PUBLIC`**: 目录对该目标和依赖它的其他目标都有效。
    - **`INTERFACE`**: 目录不适用于该目标本身，但适用于依赖它的目标。
- **`<dir1> <dir2> ...`**: 要包含的目录路径。

### **示例解释**

```cmake
target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        )
```

- **`Tutorial`** 是你的目标名称，比如一个库或可执行文件。
- **`PUBLIC`** 表示指定的包含目录不仅对 **`Tutorial`** 目标自身可见，还对那些链接了 **`Tutorial`** 的其他目标可见。这意味着任何链接了
  **`Tutorial`** 的目标都会自动获得这些包含路径，无需显式指定。
- **`"${PROJECT_BINARY_DIR}"`** 是一个变量，它包含了当前项目的二进制目录路径。这通常是 CMake 在构建过程中生成文件的地方，如
  Makefile 或项目文件。


- configure_file 官方文档

  [configure_file - CMake 3.24.0-rc5 Documentation](https://cmake.org/cmake/help/latest/command/configure_file.html)

- add_executable 官方文档

  [add_executable - CMake 3.24.0-rc5 Documentation](https://cmake.org/cmake/help/latest/command/add_executable.html?highlight=add_executable)

- target_include_directories 官方文档

  [target_include_directories - CMake 3.24.0-rc5 Documentation](https://cmake.org/cmake/help/latest/command/target_include_directories.html?highlight=target_include_directories)

- 关于 PROJECT_BINARY_DIR 变量

  [PROJECT_BINARY_DIR - CMake 3.24.0-rc5 Documentation](https://cmake.org/cmake/help/latest/variable/PROJECT_BINARY_DIR.html?highlight=project_binary_dir)

Step1/TutorialConfig.h.in

```c++
/*
 * 配置一些头文件的属性变量
 * https://cmake.org/cmake/help/latest/variable/PROJECT-NAME_VERSION_MAJOR.html
 * https://cmake.org/cmake/help/latest/variable/PROJECT-NAME_VERSION_MINOR.html
 */
#cmakedefine
Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define
Tutorial_VERSION_MINOR ${Tutorial_VERSION_MINOR}
```

在 CMake 的 **`configure_file`** 命令中，**`#define`** 和 **`#cmakedefine`** 用于在配置文件中设置预处理器指令。这些指令在
CMake 处理模板文件（如 **`TutorialConfig.h.in`**）并生成配置文件（如 **`TutorialConfig.h`**
）时非常有用。这两者的差异主要在于它们如何处理变量的定义和未定义状态。

### **`#cmakedefine`**

- **`#cmakedefine`** 用于在模板文件中创建一个条件预处理器定义。
- 当使用 **`#cmakedefine`** 指令时，如果 CMake 变量已定义（并且不是假值），预处理器定义将被包含在输出文件中。如果变量未定义或者是假值，预处理器定义将以注释的形式出现，或者完全不出现（这取决于
  CMake 的版本和配置）。

示例：

```c++
#cmakedefine Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
```

- 这表示如果 **`Tutorial_VERSION_MAJOR`** 变量在 CMake 中被定义，那么在生成的文件中将包含一行 *
  *`#define Tutorial_VERSION_MAJOR`**，后面跟着这个变量的值。
- 如果 **`Tutorial_VERSION_MAJOR`** 没有定义，那么这行将以注释形式出现或者不出现。

### **`#define`**

- **`#define`** 是常规的 C/C++ 预处理器指令，它在 CMake 的模板文件中也可以使用，但它的行为与在普通 C/C++ 源文件中无异。
- **`#define`** 在模板文件中不是条件性的。它总是会被原样复制到输出文件中，不管任何条件。

示例：

```c++
#define Tutorial_VERSION_MINOR ${Tutorial_VERSION_MINOR}
```

- 这意味着无论 **`Tutorial_VERSION_MINOR`** 变量是否在 CMake 中被定义，**`#define Tutorial_VERSION_MINOR`** 都会被包含在输出文件中。
- 如果 **`Tutorial_VERSION_MINOR`** 变量在 CMake 中被定义，其值会替换 **`${Tutorial_VERSION_MINOR}`**
  。如果未定义，这个指令可能会被替换为一个空值或者原样输出。

- 使用 **`#cmakedefine`** 时，预处理器定义的存在取决于 CMake 变量的定义状态。
- 使用 **`#define`** 时，预处理器定义总是存在，其值取决于 CMake 变量的值（或为空）。

这些功能在生成配置文件时非常有用，尤其是当你的软件需要根据不同的构建选项或环境设置来调整其行为时。

Step1/tutorial.cxx

```c++
#include <iostream>
#include <string>

#include "TutorialConfig.h"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		// 输出版本号
		std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
			<< Tutorial_VERSION_MINOR << std::endl;
		std::cout << "Usage: " << argv[0] << " [number]" << std::endl;
		return 1;
	}

	// 将输入转换为double
	const double inputValue = std::stod(argv[1]);
    std::cout << "input value: " << inputValue << std::endl;
    return 0;
}
```

[c++ std:stod 函数](https://cplusplus.com/reference/string/stod/)

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

`cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MAKE_PROGRAM=ninja -G Ninja -S .. -B .`

1. **`cmake`**: 这是调用 CMake 命令行工具的基本命令。
2. **`DCMAKE_BUILD_TYPE=Debug`**: 这个选项设置了一个 CMake 变量 **`CMAKE_BUILD_TYPE`** 的值为 **`Debug`**。这意味着构建系统将会配置为 Debug 模式，通常包括了详细的调试信息和不进行优化。
3. **`DCMAKE_MAKE_PROGRAM=ninja`**: 这里又设置了一个 CMake 变量 **`CMAKE_MAKE_PROGRAM`**，其值被设置为 **`ninja`**。这告诉 CMake 使用 Ninja 作为构建工具而不是默认的 Make。Ninja 是一个专注于速度的小型构建系统。
4. **`G Ninja`**: **`G`** 选项指定了要生成的构建系统的类型。这里指定的是 **`Ninja`**，意味着 CMake 将会生成 Ninja 构建文件。Ninja 是一种流行的高性能构建系统，与 Make 类似，但在很多情况下更快。
5. **`S ..`**: **`S`** 指定了源代码目录的路径。这里的 **`..`** 指的是当前目录的上一级目录，也就是项目的根目录。
6. **`B .`**: **`B`** 指定了用于存放构建文件（例如 Makefile 或 Ninja 构建文件）的目录。这里的 **`.`** 表示当前目录。

`cmake --build . --target Tutorial -j 6`

1. **`cmake --build .`**：
    - **`cmake --build`** 是用来调用 CMake 的构建系统的命令。
    - **`.`** 指定了构建目录，即当前目录。这意味着 CMake 将在当前目录中查找生成的构建系统文件（例如 Makefile 或 Ninja 构建文件），并使用它来构建项目。
2. **`-target Tutorial`**：
    - **`-target`** 指定了要构建的目标。在这个例子中，目标是 **`Tutorial`**。
    - **`Tutorial`** 可能是您的项目中定义的一个可执行文件、库或者自定义的目标。这个选项告诉 CMake 仅构建 **`Tutorial`** 目标，而不是整个项目的所有目标。
3. **`j 6`**：
    - **`j`** 选项用于指定并行作业的数量，在这里是 6。
    - 这意味着构建系统将尝试并行执行最多 6 个编译任务，以加快构建过程。这对于多核心计算机来说非常有用，因为它可以显著减少构建项目所需的总时间。

[返回目录](#目录)

---

## Step 2

Step2/CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)

project(Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 对于大项目，下面的操作是常见的操作。新增一个选项用于配置项目是否开启什么或关闭什么
# https://cmake.org/cmake/help/latest/command/option.html
option(USE_MY_MATH "Use tutorial provided math implementation" ON)

configure_file(TutorialConfig.h.in TutorialConfig.h)

# 根据条件是否执行某一组命令
if (USE_MY_MATH)

    # 新增子文件夹，用于添加某个想要添加的库
    # https://cmake.org/cmake/help/latest/command/add_subdirectory.html?highlight=add_subdirectory
    add_subdirectory(MathFunctions)

    # 添加两个属性，可直接用于下面作为变量使用
    # https://cmake.org/cmake/help/latest/command/list.html
    list(APPEND EXTRA_LIBS MathFunctions)
    list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif ()

add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# 可以使用下面这样的写法来直接添加一个库
# target_include_libraries(Tutorial PUBLIC MathFunctions)

target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
        # "${PROJECT_SOURCE_DIR}/MathFunctions"
        ${EXTRA_INCLUDES}
)
```

> 主要添加了一个是否使用自己定义的方法的一个 OPTION

Step2/TutorialConfig.h.in

```c++
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
#cmakedefine USE_MY_MATH
```

> 添加上述option的定义

Step2/tutorial.cxx

```c++
#include <iostream>
#include <string>

#include "TutorialConfig.h"

#ifdef USE_MY_MATH

#include <MathFunctions.h>

#endif

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
             << Tutorial_VERSION_MINOR << endl;
        cout << "Usage: " << argv[0] << " number" << endl;
        return 1;
    }

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
# 添加一个lib
# https://cmake.org/cmake/help/latest/command/add_library.html?highlight=add_library
add_library(MathFunctions mysqrt.cpp)
```

> 自己定义的function库的cmake

Step2/MathFunctions/MathFunctions.h

```c++
//
// Created by ahogek on 10/13/21.
//

#ifndef
CMAKE_LEARN_MATHFUNCTIONS_H
#define
CMAKE_LEARN_MATHFUNCTIONS_H

double mysqrt(double x);

#endif //CMAKE_LEARN_MATHFUNCTIONS_H
```

> 与库名相同的头文件可代表 library mysqrt.cpp文件的头文件来定义方法

Step2/MathFunctions/mysqrt.cpp

```c++
//
// Created by ahogek on 10/13/21.
//
#include
<iostream>

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

project(Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_MY_MATH "Use tutorial provided math implementation" ON)

configure_file(TutorialConfig.h.in TutorialConfig.h)

if (USE_MY_MATH)

    add_subdirectory(MathFunctions)

    list(APPEND EXTRA_LIBS MathFunctions)

    # 如果我们直接在MathFunctions的CMakeLists.txt下进行配置target_include_directories可省略这一步
    # list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif ()

add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
)
```

> 取消了Step2中的 "EXTRA_INCLUDES", 该操作直接在 MathFunctions 的 lib Cmake 中进行

Step3/tutorial.cxx Step3/TutorialConfig.h.in

**以上两个文件与 Step2 无异**

Step3/MathFunctions/CMakeLists.txt

```cmake
add_library(MathFunctions mysqrt.cpp)

# 任何连接这个库的人都需要包含当前源文件夹，才能找到MathFunctions.h
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

project(Tutorial VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(USE_MY_MATH "Use tutorial provided math implementation" ON)

configure_file(TutorialConfig.h.in TutorialConfig.h)

if (USE_MY_MATH)

    add_subdirectory(MathFunctions)

    list(APPEND EXTRA_LIBS MathFunctions)

endif ()

add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

target_include_directories(Tutorial PUBLIC
        "${PROJECT_BINARY_DIR}"
)

# 安装选项
# https://cmake.org/cmake/help/latest/command/install.html?highlight=install
# https://cmake.org/cmake/help/latest/guide/tutorial/Installing%20and%20Testing.html?highlight=install
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
        DESTINATION include
)

enable_testing()

# 测试程序
# https://cmake.org/cmake/help/latest/command/add_test.html?highlight=add_test
add_test(NAME Runs COMMAND Tutorial 25)

# 自定义通过规则
# https://cmake.org/cmake/help/latest/command/set_tests_properties.html?highlight=set_tests_properties
# https://cmake.org/cmake/help/latest/prop_test/PASS_REGULAR_EXPRESSION.html?highlight=pass_regular_expression
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
        PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
)

# 定义一个函数来简化添加测试
# https://cmake.org/cmake/help/latest/command/function.html?highlight=function
function(do_test target arg result)
    add_test(NAME Comp${arg} COMMAND ${target} ${arg})
    set_tests_properties(Comp${arg}
            PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction()

# 做一组结果比较测试
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
add_library(MathFunctions mysqrt.cpp)

target_include_directories(MathFunctions
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# 安装规则
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

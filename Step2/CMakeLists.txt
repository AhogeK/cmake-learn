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
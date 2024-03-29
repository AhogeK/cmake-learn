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